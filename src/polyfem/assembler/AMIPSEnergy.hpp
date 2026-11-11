#pragma once

#include <polyfem/assembler/GenericElastic.hpp>
#include <polyfem/assembler/MatParams.hpp>
#include <polyfem/utils/Types.hpp>
#include <polyfem/utils/MatrixUtils.hpp>

#include <polyfem/Common.hpp>

#include <Eigen/Dense>

#include <functional>
#include <iostream>
#include <vector>

namespace polyfem::assembler
{
	class AMIPSEnergy : public GenericElastic<AMIPSEnergy>
	{
	public:
		AMIPSEnergy();

		// sets material params
		void add_multimaterial(const int index, const json &params, const Units &units, const io::ResourceIO &resources) override;

		std::string name() const override { return "AMIPS"; }
		std::map<std::string, ParamFunc> parameters() const override;

		bool allow_inversion() const override { return false; }

		bool real_def_grad(const RowVectorNd &p, const double t, const int el_id) const override { return use_rest_pose(p, t, el_id); }

		template <typename T>
		T elastic_energy(
			const RowVectorNd &p,
			const double t,
			const int el_id,
			DefGradMatrix<T> &def_grad) const
		{
			typedef Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, 0, 3, 3> AutoDiffGradMat;

			const double power = 2. / size();
			const bool rest_pose = use_rest_pose(p, t, el_id);

			AutoDiffGradMat standard;

			if (size() == 2)
				standard = get_standard<2, T>(size(), rest_pose);
			else
				standard = get_standard<3, T>(size(), rest_pose);

			if (!rest_pose)
				def_grad = def_grad * standard;

			const T det = polyfem::utils::determinant(def_grad);
			if (det <= 0)
			{
				return T(std::nan(""));
			}

			const T powJ = pow(det, power);
			const double weight = weight_(p, t, el_id);
			const T base_energy = (def_grad.transpose() * def_grad).trace() / powJ;
			const double normalization = normalized_(p, t, el_id) != 0 ? size() : 1;
			return T(weight) * pow(base_energy / T(normalization), power_(p, t, el_id));
		}

	private:
		bool use_rest_pose(const RowVectorNd &p, const double t, const int el_id) const;
		GenericMatParam use_rest_pose_;
		GenericMatParam weight_;
		GenericMatParam power_{"power"};
		GenericMatParam normalized_{"normalized"};

		template <int dimt, class T>
		static Eigen::Matrix<T, dimt, dimt> get_standard(const int dim, const bool use_rest_pose)
		{
			Eigen::Matrix<double, dimt, dimt> standard(dim, dim);
			if (use_rest_pose)
			{
				standard.setIdentity();
			}
			else
			{
				if (dim == 2)
					standard << 1, 0,
						0.5, std::sqrt(3) / 2;
				else
					standard << 1, 0, 0,
						0.5, std::sqrt(3) / 2., 0,
						0.5, 0.5 / std::sqrt(3), std::sqrt(2. / 3.);
				standard = standard.inverse().transpose().eval();
			}

			Eigen::Matrix<T, dimt, dimt> res(dim, dim);
			for (int i = 0; i < dim; ++i)
			{
				for (int j = 0; j < dim; ++j)
				{
					res(i, j) = T(standard(i, j));
				}
			}

			return res;
		}

	public:
		/// With experiments/robust_jacobian and a conservative inversion check,
		/// det F is taken from the robust evaluation the checks use
		/// (ElementAssemblyValues::eval_deformed_jacobian_determinant), as the
		/// ratio of the reference dets of the deformed and rest maps, rather
		/// than from F in floating point; the energy, gradient and Hessian are
		/// those of W(F, J) = w (S(tr(F^T F)) / J^(2/d) / n)^power with dJ/dF
		/// from F (n = d if normalized, else 1; S the softening of
		/// experiments/softening_eps, identity if 0). Otherwise the
		/// GenericElastic path.
		using GenericElastic<AMIPSEnergy>::assemble_energy;
		using GenericElastic<AMIPSEnergy>::assemble_gradient;
		using GenericElastic<AMIPSEnergy>::assemble_hessian;
		double compute_energy(const NonLinearAssemblerData &data) const override;
		Eigen::VectorXd assemble_gradient(const NonLinearAssemblerData &data) const override;
		Eigen::MatrixXd assemble_hessian(const NonLinearAssemblerData &data) const override;

	private:
		bool uses_robust_path(const NonLinearAssemblerData &data) const;
		/// Any of the outputs may be null.
		void robust_assemble(const NonLinearAssemblerData &data, double *energy, Eigen::VectorXd *gradient, Eigen::MatrixXd *hessian) const;

	public:
		Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 3, 3> gradient(
			const RowVectorNd &p,
			const double t,
			const int el_id,
			const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 3, 3> &F) const override;

		Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 9, 9> hessian(
			const RowVectorNd &p,
			const double t,
			const int el_id,
			const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, 0, 3, 3> &F) const override;
	};

} // namespace polyfem::assembler