#include "AMIPSEnergy.hpp"

#include <polyfem/utils/Logger.hpp>
#include <polyfem/utils/Experiments.hpp>

#include <cmath>

#include <polyfem/autogen/elastic_energies/AMIPS2d.hpp>
#include <polyfem/autogen/elastic_energies/AMIPS2drest.hpp>
#include <polyfem/autogen/elastic_energies/AMIPS3d.hpp>
#include <polyfem/autogen/elastic_energies/AMIPS3drest.hpp>

namespace polyfem::assembler
{
	AMIPSEnergy::AMIPSEnergy()
		: use_rest_pose_("use_rest_pose"), weight_("weight")
	{
		autodiff_type_ = AutodiffType::NONE;
	}

	void AMIPSEnergy::add_multimaterial(const int index, const json &params, const Units &units, const io::ResourceIO &resources)
	{
		assert(size() == 2 || size() == 3);

		use_rest_pose_.add_multimaterial(index, params, "", resources);

		weight_.add_multimaterial(index, params, "", resources);

		json power_params = params;
		if (!power_params.contains("power"))
			power_params["power"] = 1.0;
		power_.add_multimaterial(index, power_params, "", resources);

		normalized_.add_multimaterial(index, params.contains("normalized") ? params : json{{"normalized", false}}, "", resources);
	}

	std::map<std::string, Assembler::ParamFunc> AMIPSEnergy::parameters() const
	{
		std::map<std::string, ParamFunc> res;

		res["use_rest_pose"] = [this](const RowVectorNd &, const RowVectorNd &p, double t, int e) {
			return use_rest_pose_(p, t, e);
		};
		res["weight"] = [this](const RowVectorNd &, const RowVectorNd &p, double t, int e) {
			return weight_(p, t, e);
		};

		res["power"] = [this](const RowVectorNd &, const RowVectorNd &p, double t, int e) {
			return power_(p, t, e);
		};

		res["normalized"] = [this](const RowVectorNd &, const RowVectorNd &p, double t, int e) {
			return normalized_(p, t, e);
		};

		return res;
	}

	bool AMIPSEnergy::use_rest_pose(const RowVectorNd &p, const double t, const int el_id) const
	{
		return use_rest_pose_(p, t, el_id) != 0;
	}

	namespace
	{
		/// dJ/dF (cofactor) and d2J/dF2, flattened column-major as vec(F).
		void det_derivatives(const Eigen::MatrixXd &F, Eigen::MatrixXd &dJ, Eigen::MatrixXd &d2J)
		{
			const int dim = F.rows();
			dJ.setZero(dim, dim);
			d2J.setZero(dim * dim, dim * dim);
			if (dim == 2)
			{
				dJ << F(1, 1), -F(1, 0),
					-F(0, 1), F(0, 0);
				d2J(0, 3) = d2J(3, 0) = 1;
				d2J(1, 2) = d2J(2, 1) = -1;
				return;
			}
			const Eigen::Vector3d u = F.col(0), v = F.col(1), w = F.col(2);
			dJ.col(0) = v.cross(w);
			dJ.col(1) = w.cross(u);
			dJ.col(2) = u.cross(v);
			const auto hat = [](const Eigen::Vector3d &x) {
				Eigen::Matrix3d m;
				m << 0, -x(2), x(1),
					x(2), 0, -x(0),
					-x(1), x(0), 0;
				return m;
			};
			d2J.block<3, 3>(0, 6) = hat(v);
			d2J.block<3, 3>(6, 0) = -hat(v);
			d2J.block<3, 3>(0, 3) = -hat(w);
			d2J.block<3, 3>(3, 0) = hat(w);
			d2J.block<3, 3>(3, 6) = -hat(u);
			d2J.block<3, 3>(6, 3) = hat(u);
		}
	} // namespace

	bool AMIPSEnergy::uses_robust_path(const NonLinearAssemblerData &data) const
	{
		if (!experiments::settings().robust_jacobian || !use_robust_jacobian)
			return false;
		for (long p = 0; p < data.da.size(); ++p)
			if (!use_rest_pose(data.vals.val.row(p), data.t, data.vals.element_id))
				return false;
		return true;
	}

	double AMIPSEnergy::compute_energy(const NonLinearAssemblerData &data) const
	{
		if (!uses_robust_path(data))
			return GenericElastic<AMIPSEnergy>::compute_energy(data);
		double energy = 0;
		robust_assemble(data, &energy, nullptr, nullptr);
		return energy;
	}

	Eigen::VectorXd AMIPSEnergy::assemble_gradient(const NonLinearAssemblerData &data) const
	{
		if (!uses_robust_path(data))
			return GenericElastic<AMIPSEnergy>::assemble_gradient(data);
		Eigen::VectorXd gradient;
		robust_assemble(data, nullptr, &gradient, nullptr);
		return gradient;
	}

	Eigen::MatrixXd AMIPSEnergy::assemble_hessian(const NonLinearAssemblerData &data) const
	{
		if (!uses_robust_path(data))
			return GenericElastic<AMIPSEnergy>::assemble_hessian(data);
		Eigen::MatrixXd hessian;
		robust_assemble(data, nullptr, nullptr, &hessian);
		return hessian;
	}

	void AMIPSEnergy::robust_assemble(const NonLinearAssemblerData &data, double *energy, Eigen::VectorXd *gradient, Eigen::MatrixXd *hessian) const
	{
		const int dim = size();
		const int n_basis = data.vals.basis_values.size();
		const int N = n_basis * dim;
		const double a = 2. / dim;

		// Midpoints of the certified enclosures of the reference det of the
		// deformed map (NaN where certainly inverted) and of the rest map, from
		// the same robust evaluation (utils::robust_evaluate_jacobian): next to
		// a collapsed corner the floating-point rest det can have the wrong sign.
		const Eigen::VectorXd jacs = data.vals.eval_deformed_jacobian_determinant(data.x);
		const Eigen::VectorXd rest_jacs = data.vals.eval_deformed_jacobian_determinant(Eigen::VectorXd::Zero(data.x.size()));

		Eigen::MatrixXd local_disp = Eigen::MatrixXd::Zero(n_basis, dim);
		for (int i = 0; i < n_basis; ++i)
			for (const auto &g : data.vals.basis_values[i].global)
				for (int d = 0; d < dim; ++d)
					local_disp(i, d) += g.val * data.x(g.index * dim + d);

		if (energy)
			*energy = 0;
		Eigen::MatrixXd G;
		if (gradient)
			G.setZero(n_basis, dim);
		if (hessian)
			hessian->setZero(N, N);

		const double nan = std::nan("");
		for (long p = 0; p < data.da.size(); ++p)
		{
			Eigen::MatrixXd grad(n_basis, dim);
			for (int i = 0; i < n_basis; ++i)
				grad.row(i) = data.vals.basis_values[i].grad.row(p);
			const Eigen::MatrixXd jac_it = data.vals.jac_it[p];
			const Eigen::MatrixXd dF_du = grad * jac_it;
			const Eigen::MatrixXd F = local_disp.transpose() * dF_du + Eigen::MatrixXd::Identity(dim, dim);

			// Physical det F: the reference det of the deformed map over that of the rest map.
			const double J = jacs(p) / rest_jacs(p);
			const RowVectorNd &pt = data.vals.val.row(p);
			const double w = weight_(pt, data.t, data.vals.element_id);
			const double k = power_(pt, data.t, data.vals.element_id);
			const double n = normalized_(pt, data.t, data.vals.element_id) != 0 ? dim : 1;

			if (!(J > 0))
			{
				// Certainly inverted (or not evaluable): as the floating-point path.
				if (energy)
					*energy = nan;
				if (gradient)
					G.setConstant(nan);
				if (hessian)
					hessian->setConstant(nan);
				break;
			}

			// W = w B^k, B = S(T) J^-a / n, T = |F|^2, with J an independent input
			// and S the softening of the numerator (identity unless eps > 0).
			const double eps = experiments::settings().softening_eps;
			const double T0 = F.squaredNorm();
			double T = T0, S1 = 1, S2 = 0; // S(T0), S'(T0), S''(T0)
			if (eps > 0 && T0 < eps)
			{
				const double r = 1 - T0 / eps;
				T = T0 + eps * r * r * r;
				S1 = 1 - 3 * r * r;
				S2 = 6 * r / eps;
			}
			const double Ja = std::pow(J, -a) / n;
			const double B = T * Ja;
			if (energy)
			{
				// With subtract_rest_value, B^k - R^k with R = d / n the minimum of B,
				// as R^k expm1(k log(B / R)): no cancellation near rest.
				const double R = dim / n;
				*energy += w * (experiments::settings().subtract_rest_value ? std::pow(R, k) * std::expm1(k * std::log(B / R)) : std::pow(B, k)) * data.da(p);
			}
			if (!gradient && !hessian)
				continue;

			Eigen::MatrixXd dJ, d2J;
			det_derivatives(F, dJ, d2J);
			const Eigen::Map<const Eigen::VectorXd> f(F.data(), F.size());
			const Eigen::Map<const Eigen::VectorXd> c(dJ.data(), dJ.size());
			// dB/dF = 2 S' F J^-a / n - a (B / J) dJ/dF
			const Eigen::VectorXd dB = 2 * S1 * Ja * f - a * B / J * c;

			if (gradient)
			{
				const Eigen::VectorXd dW = w * k * std::pow(B, k - 1) * dB;
				const Eigen::Map<const Eigen::MatrixXd> dW_dF(dW.data(), dim, dim);
				G.noalias() += dF_du * dW_dF.transpose() * data.da(p);
			}

			if (hessian)
			{
				// d2B = 2 S' J^-a/n I + 4 S'' J^-a/n f f^T - 2 a S' J^(-a-1)/n (c f^T + f c^T)
				//       + S (a (a+1) J^(-a-2)/n c c^T - a J^(-a-1)/n d2J)
				const double Ja1 = Ja / J, Ja2 = Ja1 / J;
				Eigen::MatrixXd d2B = 2 * S1 * Ja * Eigen::MatrixXd::Identity(dim * dim, dim * dim);
				if (S2 != 0)
					d2B.noalias() += 4 * S2 * Ja * (f * f.transpose());
				d2B.noalias() -= 2 * a * S1 * Ja1 * (c * f.transpose() + f * c.transpose());
				d2B.noalias() += T * (a * (a + 1) * Ja2 * (c * c.transpose()) - a * Ja1 * d2J);
				Eigen::MatrixXd d2W = w * k * std::pow(B, k - 1) * d2B;
				if (k != 1)
					d2W.noalias() += w * k * (k - 1) * std::pow(B, k - 2) * (dB * dB.transpose());

				// vec(F) = sum over (node i, coordinate j) of e_j (x) dF_du.row(i)
				Eigen::MatrixXd dF = Eigen::MatrixXd::Zero(dim * dim, N);
				for (int i = 0; i < n_basis; ++i)
					for (int j = 0; j < dim; ++j)
						for (int col = 0; col < dim; ++col)
							dF(col * dim + j, i * dim + j) = dF_du(i, col);
				hessian->noalias() += dF.transpose() * d2W * dF * data.da(p);
			}
		}

		if (gradient)
		{
			const Eigen::MatrixXd G_T = G.transpose();
			*gradient = Eigen::Map<const Eigen::VectorXd>(G_T.data(), G_T.size());
		}
	}

	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 3, 3> AMIPSEnergy::gradient(
		const RowVectorNd &p,
		const double t,
		const int el_id,
		const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 3, 3> &F) const
	{
		const double det = polyfem::utils::determinant(F);
		if (det <= 0)
		{
			Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 3, 3> grad(size(), size());
			grad.setConstant(std::nan(""));
			return grad;
		}

		const double weight = weight_(p, t, el_id);
		const double power = power_(p, t, el_id);
		const double normalization = normalized_(p, t, el_id) != 0 ? size() : 1;
		if (use_rest_pose(p, t, el_id))
		{
			if (size() == 2)
				return weight * autogen::AMIPS2drest_gradient(p, t, el_id, F, power, normalization);
			else
				return weight * autogen::AMIPS3drest_gradient(p, t, el_id, F, power, normalization);
		}
		else
		{
			if (size() == 2)
				return weight * autogen::AMIPS2d_gradient(p, t, el_id, F, power, normalization);
			else
				return weight * autogen::AMIPS3d_gradient(p, t, el_id, F, power, normalization);
		}
	}

	Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 9, 9> AMIPSEnergy::hessian(
		const RowVectorNd &p,
		const double t,
		const int el_id,
		const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 3, 3> &F) const
	{
		const double det = polyfem::utils::determinant(F);
		if (det <= 0)
		{
			Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 9, 9> hessian(size() * size(), size() * size());
			hessian.setConstant(std::nan(""));
			return hessian;
		}

		const double weight = weight_(p, t, el_id);
		const double power = power_(p, t, el_id);
		const double normalization = normalized_(p, t, el_id) != 0 ? size() : 1;
		if (use_rest_pose(p, t, el_id))
		{
			if (size() == 2)
				return weight * autogen::AMIPS2drest_hessian(p, t, el_id, F, power, normalization);
			else
				return weight * autogen::AMIPS3drest_hessian(p, t, el_id, F, power, normalization);
		}
		else
		{
			if (size() == 2)
				return weight * autogen::AMIPS2d_hessian(p, t, el_id, F, power, normalization);
			else
				return weight * autogen::AMIPS3d_hessian(p, t, el_id, F, power, normalization);
		}
	}

} // namespace polyfem::assembler