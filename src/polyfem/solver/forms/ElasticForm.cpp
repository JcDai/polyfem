#include "ElasticForm.hpp"

#include <polyfem/quadrature/TriQuadrature.hpp>
#include <polyfem/quadrature/TetQuadrature.hpp>
#include <polyfem/assembler/AssemblerUtils.hpp>
#include <polyfem/basis/ElementBases.hpp>
#include <polyfem/assembler/MatParams.hpp>
#include <polyfem/utils/Logger.hpp>
#include <polyfem/utils/Timer.hpp>
#include <polyfem/utils/Jacobian.hpp>
#include <polyfem/utils/Experiments.hpp>

#include <algorithm>
#include <functional>
#include <map>
#include <cassert>
#include <cmath>
#include <memory>
#include <stdexcept>
#include <tuple>
#include <limits>
#include <vector>

using namespace polyfem::assembler;
using namespace polyfem::utils;
using namespace polyfem::quadrature;

namespace polyfem::solver
{
	namespace
	{
		// Conservative modes use corner rules so the energy samples J at the
		// corners. A negative weight at any point turns the log barrier there
		// into a well, and the discrete energy becomes unbounded below as J
		// vanishes at that point: the triangle order-6 rule (P4) has them at
		// the corners, the tet order-4 rule (P3) at the centroid. Use the
		// lowest order at or above the requested one whose rule has every
		// corner and only positive weights. A family of corner rules with
		// positive weights at every order would avoid the larger rules.
		Quadrature corner_quadrature(const int dim, const int order)
		{
			const auto is_usable = [dim](const Quadrature &quad) {
				if (quad.weights.size() == 0 || quad.weights.minCoeff() <= 0)
					return false;
				int n_corners = 0;
				for (Eigen::Index i = 0; i < quad.points.rows(); ++i)
				{
					const auto p = quad.points.row(i);
					if ((p.array() == 0 || p.array() == 1).all() && p.sum() <= 1)
						++n_corners;
				}
				return n_corners == dim + 1;
			};

			constexpr int max_bump = 3;
			Quadrature quad;
			for (int o = order; o <= order + max_bump; ++o)
			{
				if (dim == 2)
					TriQuadrature(true).get_quadrature(o, quad);
				else
					TetQuadrature(true).get_quadrature(o, quad);
				if (is_usable(quad))
					return quad;
			}
			log_and_throw_error("No corner quadrature with positive weights of order {} to {} in {}D", order, order + max_bump, dim);
		}

		/// Quadrature of the degenerate checks' rule (utils::degenerate_quadrature)
		/// copied onto every leaf of @p tree. Each point is computed as
		/// lambda_0 v_0 + sum_j mu_j v_j with lambda_0 = 1 - sum_j mu_j, the leaf
		/// vertices v_k and the rule's point mu, in double precision: the
		/// rounding the checks' samples are widened by (pymiso sample_points)
		/// assumes exactly this.
		quadrature::Quadrature refine_degenerate_quadrature(const Tree &tree, const int dim)
		{
			Eigen::MatrixXd pts = Eigen::MatrixXd::Zero(dim + 1, dim);
			for (int k = 1; k <= dim; ++k)
				pts(k, k - 1) = 1;
			const auto [leaf_vertices, exponents] = extract_subelement_volumes(pts, tree);
			const Quadrature rule = degenerate_quadrature(dim);
			const int n = rule.size();

			Quadrature quad;
			quad.points.resize(n * exponents.size(), dim);
			quad.weights.resize(n * exponents.size());
			for (size_t l = 0; l < exponents.size(); ++l)
			{
				const auto v = leaf_vertices.middleRows(l * (dim + 1), dim + 1);
				for (int q = 0; q < n; ++q)
				{
					const Eigen::RowVectorXd mu = rule.points.row(q);
					const double lambda0 = 1. - mu.sum();
					Eigen::RowVectorXd x = lambda0 * v.row(0);
					for (int j = 0; j < dim; ++j)
						x += mu(j) * v.row(j + 1);
					quad.points.row(l * n + q) = x;
					quad.weights(l * n + q) = std::ldexp(rule.weights(q), -exponents[l]);
				}
			}
			assert(fabs(quad.weights.sum() - rule.weights.sum()) < 1e-8);
			return quad;
		}

		quadrature::Quadrature refine_quadrature(const Tree &tree, const int dim, const int order)
		{
			Eigen::MatrixXd pts(dim + 1, dim);
			if (dim == 2)
				pts << 0., 0.,
					1., 0.,
					0., 1.;
			else
				pts << 0, 0, 0,
					1, 0, 0,
					0, 1, 0,
					0, 0, 1;
			auto [quad_points, levels] = extract_subelement(pts, tree);

			Quadrature tmp = corner_quadrature(dim, order);
			Quadrature quad;
			tmp.points.conservativeResize(tmp.points.rows(), dim + 1);
			tmp.points.col(dim) = 1. - tmp.points.leftCols(dim).rowwise().sum().array();

			quad.points.resize(tmp.size() * levels.size(), dim);
			quad.weights.resize(tmp.size() * levels.size());

			for (int i = 0; i < levels.size(); i++)
			{
				quad.points.middleRows(i * tmp.size(), tmp.size()) = tmp.points * quad_points.middleRows(i * (dim + 1), dim + 1);
				quad.weights.segment(i * tmp.size(), tmp.size()) = tmp.weights / pow(2, dim * levels[i]);
			}
			assert(fabs(quad.weights.sum() - tmp.weights.sum()) < 1e-8);

			return quad;
		}

		void update_quadrature(const int invalidID, const int dim, Tree &tree, const int quad_order, basis::ElementBases &bs, const basis::ElementBases &gbs, assembler::AssemblyValsCache &ass_vals_cache, const bool degenerate = false)
		{
			// update quadrature to capture the point with negative jacobian
			const Quadrature quad = degenerate ? refine_degenerate_quadrature(tree, dim) : refine_quadrature(tree, dim, quad_order);

			// capture the flipped point by refining the quadrature
			bs.set_quadrature([quad](Quadrature &quad_) {
				quad_ = quad;
			});
			logger().trace("New number of quadrature points: {}, level: {}", quad.size(), tree.depth());

			if (ass_vals_cache.is_initialized())
				ass_vals_cache.update(invalidID, dim == 3, bs, gbs);
		}

		Eigen::MatrixXd ref_vertices(const int dim)
		{
			Eigen::MatrixXd V = Eigen::MatrixXd::Zero(dim + 1, dim);
			for (int k = 1; k <= dim; ++k)
				V(k, k - 1) = 1;
			return V;
		}

		/// Vertices of child @p i of the simplex @p V split with @p scheme.
		Eigen::MatrixXd child_vertices(const Eigen::MatrixXd &V, const int scheme, const int i)
		{
			const int dim = V.cols();
			Eigen::MatrixXd B = Eigen::MatrixXd::Zero(dim + 1, dim + 1);
			B.rightCols(dim) = child_nodes(dim, scheme, i);
			B.col(0) = 1. - B.rightCols(dim).rowwise().sum().array();
			return B * V;
		}

		/// Euclidean projection onto the reference simplex {p >= 0, sum p <= 1}.
		Eigen::RowVectorXd project_simplex(const Eigen::RowVectorXd &p)
		{
			const int n = p.size();
			Eigen::VectorXd lam(n + 1);
			lam(0) = 1 - p.sum();
			lam.tail(n) = p.transpose();
			std::vector<double> u(lam.data(), lam.data() + n + 1);
			std::sort(u.begin(), u.end(), std::greater<double>());
			double css = 0, theta = 0;
			for (int j = 0; j <= n; ++j)
			{
				css += u[j];
				const double t = (css - 1) / (j + 1);
				if (u[j] - t > 0)
					theta = t;
			}
			lam = (lam.array() - theta).max(0.0);
			return lam.tail(n).transpose();
		}

		/// Minimize f over the reference simplex from p: finite-difference Newton
		/// with backtracking (step h), then a compass search.
		std::pair<Eigen::RowVectorXd, double> simplex_minimize(
			const std::function<Eigen::VectorXd(const Eigen::MatrixXd &)> &f,
			Eigen::RowVectorXd p, const double h, const int newton_iters, int *n_newton = nullptr)
		{
			const int n = p.size();
			p = project_simplex(p);
			auto f1 = [&](const Eigen::RowVectorXd &q) { return f(Eigen::MatrixXd(q))(0); };
			double fp = f1(p);
			int it = 0;
			for (; it < newton_iters; ++it)
			{
				const int npts = 1 + 2 * n + 2 * n * (n - 1);
				Eigen::MatrixXd S(npts, n);
				int r = 0;
				S.row(r++) = p;
				for (int i = 0; i < n; ++i)
				{
					Eigen::RowVectorXd e = Eigen::RowVectorXd::Zero(n);
					e(i) = h;
					S.row(r++) = p + e;
					S.row(r++) = p - e;
				}
				for (int i = 0; i < n; ++i)
					for (int j = i + 1; j < n; ++j)
					{
						Eigen::RowVectorXd ei = Eigen::RowVectorXd::Zero(n), ej = Eigen::RowVectorXd::Zero(n);
						ei(i) = h;
						ej(j) = h;
						S.row(r++) = p + ei + ej;
						S.row(r++) = p + ei - ej;
						S.row(r++) = p - ei + ej;
						S.row(r++) = p - ei - ej;
					}
				const Eigen::VectorXd fv = f(S);
				if (!fv.allFinite())
					break;
				Eigen::VectorXd g(n);
				Eigen::MatrixXd H(n, n);
				for (int i = 0; i < n; ++i)
				{
					g(i) = (fv(1 + 2 * i) - fv(2 + 2 * i)) / (2 * h);
					H(i, i) = (fv(1 + 2 * i) - 2 * fv(0) + fv(2 + 2 * i)) / (h * h);
				}
				r = 1 + 2 * n;
				for (int i = 0; i < n; ++i)
					for (int j = i + 1; j < n; ++j)
					{
						H(i, j) = H(j, i) = (fv(r) - fv(r + 1) - fv(r + 2) + fv(r + 3)) / (4 * h * h);
						r += 4;
					}
				Eigen::VectorXd d;
				Eigen::LLT<Eigen::MatrixXd> llt(H);
				if (llt.info() == Eigen::Success)
					d = llt.solve(-g);
				if (d.size() == 0 || !d.allFinite() || d.dot(g) >= 0)
					d = -g;
				if (d.norm() > 0.25)
					d *= 0.25 / d.norm();
				bool moved = false;
				for (double a = 1; a > 1e-12; a /= 2)
				{
					const Eigen::RowVectorXd q = project_simplex(p + a * d.transpose());
					const double fq = f1(q);
					if (q.allFinite() && fq < fp)
					{
						moved = (q - p).norm() > 1e-14;
						p = q;
						fp = fq;
						break;
					}
				}
				if (!moved)
					break;
			}
			if (n_newton)
				*n_newton = it;

			std::vector<Eigen::RowVectorXd> dirs;
			for (int k = 0; k < n; ++k)
			{
				Eigen::RowVectorXd d = Eigen::RowVectorXd::Zero(n);
				d(k) = 1;
				dirs.push_back(d);
				dirs.push_back(-d);
			}
			dirs.push_back(Eigen::RowVectorXd::Constant(n, 1. / std::sqrt(n)));
			dirs.push_back(Eigen::RowVectorXd::Constant(n, -1. / std::sqrt(n)));
			for (double step = 1e-2; step > 1e-10;)
			{
				Eigen::MatrixXd C(dirs.size(), n);
				for (size_t i = 0; i < dirs.size(); ++i)
					C.row(i) = project_simplex(p + step * dirs[i]);
				const Eigen::VectorXd fc = f(C);
				int best = -1;
				for (int i = 0; i < fc.size(); ++i)
					if (std::isfinite(fc(i)) && fc(i) < fp && (best < 0 || fc(i) < fc(best)))
						best = i;
				if (best >= 0)
				{
					p = C.row(best);
					fp = fc(best);
				}
				else
					step /= 2;
			}
			return {p, fp};
		}
	} // namespace

	ElasticForm::ElasticForm(const int n_bases,
							 std::vector<basis::ElementBases> &bases,
							 const std::vector<basis::ElementBases> &geom_bases,
							 const assembler::Assembler &assembler,
							 assembler::AssemblyValsCache &ass_vals_cache,
							 const double t, const double dt,
							 const bool is_volume,
							 const double jacobian_threshold,
							 const ElementInversionCheck check_inversion,
							 const unsigned conservative_max_iter)
		: n_bases_(n_bases),
		  bases_(bases),
		  geom_bases_(geom_bases),
		  assembler_(assembler),
		  ass_vals_cache_(ass_vals_cache),
		  t_(t),
		  jacobian_threshold_(jacobian_threshold),
		  check_inversion_(check_inversion),
		  conservative_max_iter_(conservative_max_iter),
		  dt_(dt),
		  is_volume_(is_volume)
	{
		if (assembler_.is_linear())
			compute_cached_stiffness();
		// mat_cache_ = std::make_unique<utils::DenseMatrixCache>();
		mat_cache_ = std::make_unique<utils::SparseMatrixCache>();
		quadrature_hierarchy_.resize(bases_.size());

		quadrature_order_ = AssemblerUtils::quadrature_order(assembler_.name(), bases_[0].bases[0].order(), AssemblerUtils::BasisType::SIMPLEX_LAGRANGE, is_volume_ ? 3 : 2);

		if (is_conservative(check_inversion_))
		{
			// The rest state is certified valid at the first check
			// (validate_rest()), once degenerate corners, if any, are loaded.
			rest_nodes_ = utils::rest_nodes(is_volume_ ? 3 : 2, bases_, geom_bases_);
			// The energy evaluates det J from the same control points the
			// checks see; compute them once rather than on every assembly.
			ass_vals_cache_.set_control_points(utils::element_control_points(is_volume_ ? 3 : 2, bases_, geom_bases_));

			int basis_order = 0;
			int gbasis_order = 0;
			for (int e = 0; e < bases_.size(); e++)
			{
				if (basis_order == 0)
					basis_order = bases_[e].bases.front().order();
				else if (basis_order != bases_[e].bases.front().order())
					log_and_throw_error("Non-uniform basis order not supported for conservative Jacobian check!!");
				if (gbasis_order == 0)
					gbasis_order = geom_bases_[e].bases.front().order();
				else if (gbasis_order != geom_bases_[e].bases.front().order())
					log_and_throw_error("Non-uniform gbasis order not supported for conservative Jacobian check!!");
			}

			refines_quadrature_ = basis_order > 1 || gbasis_order > 1;

			// Replace the corner quadrature set by LagrangeBasis for every element.
			// Some default corner rules have negative weights (see
			// corner_quadrature); override them here so the base quadrature is
			// also safe before any adaptive refinement occurs.
			const int dim = is_volume_ ? 3 : 2;
			if (refines_quadrature_)
				for (int e = 0; e < (int)bases_.size(); e++)
					update_quadrature(e, dim, quadrature_hierarchy_[e], quadrature_order_, bases_[e], geom_bases_[e], ass_vals_cache_);
		}
	}

	double ElasticForm::value_unweighted(const Eigen::VectorXd &x) const
	{
		return assembler_.assemble_energy(
			is_volume_,
			bases_, geom_bases_, ass_vals_cache_, t_, dt_, x, x_prev_);
	}

	Eigen::VectorXd ElasticForm::value_per_element_unweighted(const Eigen::VectorXd &x) const
	{
		const Eigen::VectorXd out = assembler_.assemble_energy_per_element(
			is_volume_, bases_, geom_bases_, ass_vals_cache_, t_, dt_, x, x_prev_);
		assert(abs(out.sum() - value_unweighted(x)) < std::max(1e-10 * out.sum(), 1e-10));
		return out;
	}

	void ElasticForm::first_derivative_unweighted(const Eigen::VectorXd &x, Eigen::VectorXd &gradv) const
	{
		Eigen::MatrixXd grad;
		assembler_.assemble_gradient(is_volume_, n_bases_, bases_, geom_bases_,
									 ass_vals_cache_, t_, dt_, x, x_prev_, grad);
		gradv = grad;
	}

	void ElasticForm::second_derivative_unweighted(const Eigen::VectorXd &x, StiffnessMatrix &hessian) const
	{
		POLYFEM_SCOPED_TIMER("elastic hessian");

		hessian.resize(x.size(), x.size());

		if (assembler_.is_linear())
		{
			assert(cached_stiffness_.rows() == x.size() && cached_stiffness_.cols() == x.size());
			hessian = cached_stiffness_;
		}
		else
		{
			// NOTE: mat_cache_ is marked as mutable so we can modify it here
			assembler_.assemble_hessian(
				is_volume_, n_bases_, project_to_psd_, bases_,
				geom_bases_, ass_vals_cache_, t_, dt_, x, x_prev_, *mat_cache_, hessian);
		}
	}

	void ElasticForm::finish()
	{
		// Report what the conservative check cost over this solve. The call
		// counts matter as much as the totals: the static check runs once per
		// line-search trial point, the continuous one once per line search.
		if (static_check_calls_ > 0)
		{
			logger().debug(
				"Static Jacobian check: {} calls, {} rejections, {:.3f}s total ({:.3f} ms/call)",
				static_check_calls_, static_check_rejections_, static_check_time_,
				1e3 * static_check_time_ / static_check_calls_);
		}
		if (continuous_check_calls_ > 0)
		{
			logger().debug(
				"Continuous Jacobian check: {} calls, {:.3f}s total ({:.3f} ms/call)",
				continuous_check_calls_, continuous_check_time_,
				1e3 * continuous_check_time_ / continuous_check_calls_);
		}
		static_check_time_ = continuous_check_time_ = 0;
		static_check_calls_ = continuous_check_calls_ = static_check_rejections_ = 0;

		for (auto &t : quadrature_hierarchy_)
			t = Tree();
		for (auto &d : deep_dirs_)
			d.clear();
		pending_refinement_.reset();
		recompute_ = false;
		retries_ = 0;
	}

	double ElasticForm::max_step_size(const Eigen::VectorXd &x0, const Eigen::VectorXd &x1) const
	{
		// Only the continuous check caps the step: it is the one that certifies
		// the whole segment. The static check instead rejects individual
		// candidates from is_step_valid(), so it leaves the step alone here.
		if (check_inversion_ != ElementInversionCheck::ConservativeContinuous)
			return 1.;
		validate_rest();

		const int dim = is_volume_ ? 3 : 2;

		// Largest gap allowed between the certified step and the first
		// inversion, as a fraction of the step. It only bounds the gap from
		// above: when the inversion starts away from a subdivision vertex the
		// check cannot stop until a sample shows it, by which point the gap is
		// already far smaller. Keep it small enough that the margin applied
		// below, rather than this slack, sets how far a capped step stays
		// from the inversion.
		constexpr double precision = 1e-2;

		double step, invalidStep;
		int invalidID;

		Tree subdivision_tree;
		{
			double transient_check_time = 0;
			{
				POLYFEM_SCOPED_TIMER("Transient Jacobian Check", transient_check_time);
				std::tie(step, invalidID, invalidStep, subdivision_tree) = max_time_step(dim, bases_, geom_bases_, x0, x1, precision, jacobian_threshold_, conservative_max_iter_, &rest_nodes_, degenerate_.get());
			}

			continuous_check_time_ += transient_check_time;
			++continuous_check_calls_;

			logger().log(step == 0 ? spdlog::level::err : (step == 1. ? spdlog::level::trace : spdlog::level::debug),
						 "Jacobian max step size: {} at element {}, invalid step size: {}, runtime {} sec, tree depth {}", step, invalidID, invalidStep, transient_check_time, subdivision_tree.depth());
		}

		// Stop short of the certified step (see the end of this function).
		constexpr double margin = 0.9;

		// experiments/check_inversion_retry: refine at x0 now and, if the
		// quadrature changed, ask the solver to recompute the direction at x0
		// instead of taking the truncated step. A step is taken once the
		// quadrature no longer changes, or after max_retries.
		const auto &retry = experiments::settings().check_inversion_retry;
		if (retry.enabled && invalidID >= 0 && retries_ < retry.max_retries)
		{
			// Refining next to a designed zero (degenerate corner) can put a
			// point where det F is 0/0 at x0; undo such a refinement.
			const Tree saved = quadrature_hierarchy_[invalidID];
			deep_locate_x_ = x0 + step * (x1 - x0); // where the check found det F about to vanish
			deep_guard_x_ = x0;
			if (commit_refinement(invalidID, subdivision_tree))
			{
				if (std::isfinite(element_energy(invalidID, x0)))
				{
					recompute_ = true;
					++retries_;
					pending_refinement_.reset();
					logger().debug("Retry: quadrature of element {} refined at x0; recomputing the direction (retry {})", invalidID, retries_);
					return step < 1 ? margin * step : step;
				}
				logger().debug("Retry: refinement of element {} makes its energy non-finite at x0; reverted", invalidID);
				quadrature_hierarchy_[invalidID] = saved;
				rebuild_element_quadrature(invalidID);
				// Do not keep the same refinement as the pending candidate:
				// committed after the step, it would make the energy non-finite.
				pending_refinement_.reset();
				return step < 1 ? margin * step : step;
			}
		}

		if (invalidID >= 0 && step == 0)
		{
			// A step of 0 can never be accepted, so it will never reach
			// post_step(); commit the refinement candidate immediately so
			// the next iteration's energy/gradient/Hessian actually sees
			// this near-degenerate point instead of repeating the same
			// failure against stale quadrature.
			deep_locate_x_ = deep_guard_x_ = x0;
			commit_refinement(invalidID, subdivision_tree);
			pending_refinement_.reset();
		}
		else if (invalidID >= 0)
		{
			// Any capped step: the path inverts somewhere the quadrature does
			// not sample, and left that way the next direction heads back to
			// it. Store as a candidate only; committed in post_step() if/when
			// this step is accepted (see pending_refinement_ in the header).
			pending_refinement_ = std::make_pair(invalidID, std::move(subdivision_tree));
		}
		else
		{
			pending_refinement_.reset();
		}

		// Stop short of the certified step. It can lie arbitrarily close to the
		// first inversion, and a step taken all the way to it can leave the
		// Jacobian somewhere within rounding of zero. No later check can then
		// certify any step from there, and the solve stalls. The margin is still
		// inside the certified range, which the widening in max_time_step()
		// covers at any correctly rounded point.
		return step < 1 ? margin * step : step;
	}

	bool ElasticForm::is_step_collision_free(const Eigen::VectorXd &x0, const Eigen::VectorXd &x1) const
	{
		// NOTE: this hook is named for contact, but ElasticForm uses it to mean
		// "no element is inverted". polysolve's line search never calls it;
		// ALSolver does, on whole configurations, a handful of times per solve.
		// The line search is covered by max_step_size() (continuous) and
		// is_step_acceptable() (static).
		if (!is_conservative(check_inversion_))
			return true;

		validate_rest();
		const auto [isvalid, id, tree] = is_valid(is_volume_ ? 3 : 2, bases_, geom_bases_, x1, jacobian_threshold_, conservative_max_iter_, &rest_nodes_, degenerate_.get());
		return isvalid;
	}

	bool ElasticForm::is_step_valid(const Eigen::VectorXd &x0, const Eigen::VectorXd &x1) const
	{
		// check inversion on quadrature points
		Eigen::VectorXd grad;
		first_derivative(x1, grad);
		if (grad.array().isNaN().any())
			return false;

		return true;

		// Check the scalar field in the output does not contain NANs.
		// WARNING: Does not work because the energy is not evaluated at the same quadrature points.
		//          This causes small step lengths in the LS.
		// TVector x1_full;
		// reduced_to_full(x1, x1_full);
		// return state_.check_scalar_value(x1_full, true, false);
		// return true;
	}

	bool ElasticForm::is_step_acceptable(const Eigen::VectorXd &x0, const Eigen::VectorXd &x1) const
	{
		// The static check certifies one configuration, the same question the
		// quadrature sampling answers, but over the whole element. It runs only
		// on steps that already passed the line search's descent criteria:
		// running it on every trial point costs far more, and vetoing a trial
		// the energy cannot tell apart from a valid one leaves the line search
		// nothing to back away from.
		if (check_inversion_ != ElementInversionCheck::ConservativeStatic)
			return true;
		validate_rest();

		double check_time = 0;
		bool isvalid = false;
		int id = -1;
		Tree witness;
		{
			POLYFEM_SCOPED_TIMER("Static Jacobian Check", check_time);
			std::tie(isvalid, id, witness) = is_valid(
				is_volume_ ? 3 : 2, bases_, geom_bases_, x1, jacobian_threshold_, conservative_max_iter_, &rest_nodes_, degenerate_.get());
		}
		static_check_time_ += check_time;
		++static_check_calls_;

		if (isvalid)
			return true;

		++static_check_rejections_;
		logger().debug("Static Jacobian check rejected step at element {} (witness depth {})", id, witness.depth());

		// Refine towards the violation, so that energy evaluated at this
		// configuration samples it and is no longer finite. A rejection ends
		// the line search, and the next direction is computed on the refined
		// quadrature, so a later trial that reaches this configuration is
		// turned down by the line search's ordinary finiteness checks. Merge
		// the whole witness path rather than the usual two levels: the
		// violation is certified at a vertex of the deepest witness simplex,
		// and that vertex is only a quadrature point (corner quadrature) once
		// that level of refinement exists.
		if (id >= 0)
		{
			deep_locate_x_ = deep_guard_x_ = x0;
			commit_refinement(id, witness, std::numeric_limits<int>::max());
		}

		return false;
	}

	void ElasticForm::post_step(const polysolve::nonlinear::PostStepData &data)
	{
		retries_ = 0;
		if (!pending_refinement_.has_value())
			return;

		auto [id, subdivision_tree] = *pending_refinement_;
		pending_refinement_.reset();

		// As in the retry: next to a designed zero (degenerate corner) a
		// refinement can put a point where det F is 0/0 at the accepted
		// state; keep the previous quadrature then.
		const Tree saved = quadrature_hierarchy_[id];
		deep_locate_x_ = deep_guard_x_ = data.x;
		if (commit_refinement(id, subdivision_tree) && !std::isfinite(element_energy(id, data.x)))
		{
			logger().debug("Refinement of element {} makes its energy non-finite; reverted", id);
			quadrature_hierarchy_[id] = saved;
			rebuild_element_quadrature(id);
		}
	}

	const utils::DegenerateCorner &ElasticForm::corner(const int e) const
	{
		static const utils::DegenerateCorner none;
		return degenerate_ ? degenerate_->corners[e] : none;
	}

	int ElasticForm::corner_basis(const int e) const
	{
		const int dim = is_volume_ ? 3 : 2;
		ElementAssemblyValues vals;
		vals.compute(e, is_volume_, Eigen::MatrixXd(ref_vertices(dim).row(corner(e).vertex)), bases_[e], geom_bases_[e]);
		for (int i = 0; i < (int)vals.basis_values.size(); ++i)
			if (std::abs(vals.basis_values[i].val(0) - 1) < 1e-12)
				return i;
		log_and_throw_error("No basis is 1 at the degenerate corner of element {}", e);
		return -1;
	}

	Eigen::MatrixXd ElasticForm::local_disp(const int e, const Eigen::VectorXd &x) const
	{
		const int dim = is_volume_ ? 3 : 2;
		const auto &bs = bases_[e];
		Eigen::MatrixXd u = Eigen::MatrixXd::Zero(bs.bases.size(), dim);
		for (size_t i = 0; i < bs.bases.size(); ++i)
			for (const auto &g : bs.bases[i].global())
				u.row(i) += g.val * x.segment(g.index * dim, dim).transpose();
		return u;
	}

	std::vector<Eigen::MatrixXd> ElasticForm::limit_grads(const int e, const Eigen::MatrixXd &dirs) const
	{
		const int dim = is_volume_ ? 3 : 2;
		const utils::DegenerateCorner &c = corner(e);
		const Eigen::RowVectorXd v = ref_vertices(dim).row(c.vertex);
		const int nd = dirs.rows();
		Eigen::MatrixXd pts(3 * nd, dim);
		for (int k = 0; k < nd; ++k)
		{
			pts.row(3 * k) = v + limit_r_ * dirs.row(k);
			pts.row(3 * k + 1) = v + 2 * limit_r_ * dirs.row(k);
			pts.row(3 * k + 2) = v + 4 * limit_r_ * dirs.row(k);
		}
		ElementAssemblyValues vals;
		vals.compute(e, is_volume_, pts, bases_[e], geom_bases_[e]);
		const int n = vals.basis_values.size();
		const int vb = corner_basis(e);
		std::vector<Eigen::MatrixXd> out(nd, Eigen::MatrixXd(n, dim));
		for (int k = 0; k < nd; ++k)
		{
			auto &G = out[k];
			for (int i = 0; i < n; ++i)
			{
				const auto &g = vals.basis_values[i].grad_t_m;
				// Richardson: F(r) = F0 + c1 r + c2 r^2 + O(r^3)
				G.row(i) = (8 * g.row(3 * k) - 6 * g.row(3 * k + 1) + g.row(3 * k + 2)) / 3.;
			}
			// The collapsed control points merge into the corner: only their sum
			// has a limit (the displacement keeps the collapse).
			for (const int j : c.near_nodes)
			{
				G.row(vb) += G.row(j);
				G.row(j).setZero();
			}
		}
		return out;
	}

	Eigen::VectorXd ElasticForm::limit_det(const int e, const Eigen::MatrixXd &dirs, const Eigen::VectorXd &x) const
	{
		const int dim = is_volume_ ? 3 : 2;
		const std::vector<Eigen::MatrixXd> G = limit_grads(e, dirs);
		const Eigen::MatrixXd u = local_disp(e, x);
		Eigen::VectorXd out(dirs.rows());
		for (int k = 0; k < dirs.rows(); ++k)
		{
			const Eigen::MatrixXd F = Eigen::MatrixXd::Identity(dim, dim) + u.transpose() * G[k];
			out(k) = F.allFinite() ? F.determinant() : std::nan("");
		}
		return out;
	}

	bool ElasticForm::deep_commit(const int id, const utils::Tree &witness) const
	{
		const int dim = is_volume_ ? 3 : 2;
		const int D = experiments::settings().deep_leaf_depth;
		if (deep_dirs_.size() != bases_.size())
			deep_dirs_.resize(bases_.size());
		const Tree saved_tree = quadrature_hierarchy_[id];
		const auto saved_dirs = deep_dirs_[id];

		// The witness, cut at absolute depth D, merged in full.
		std::function<void(const Tree &, Tree &, int)> trunc = [&](const Tree &w, Tree &o, const int d) {
			if (!w.has_children() || d >= D)
				return;
			o.add_children(w.n_children(), w.scheme());
			for (int i = 0; i < w.n_children(); ++i)
				trunc(w.child(i), o.child(i), d + 1);
		};
		Tree cut;
		trunc(witness, cut, 0);
		bool changed = quadrature_hierarchy_[id].merge(cut, std::numeric_limits<int>::max());

		// A deeper witness in a leaf touching the degenerate corner: one
		// directional node for the depth-D leaf on its deepest path.
		const utils::DegenerateCorner &c = corner(id);
		const Eigen::VectorXd &xl = deep_locate_x_;
		if (c.vertex >= 0 && witness.depth() > D && xl.size() == n_bases_ * dim)
		{
			Eigen::MatrixXd V = ref_vertices(dim);
			const Tree *h = &quadrature_hierarchy_[id], *w = &witness;
			std::vector<int> path;
			while (h->has_children() && w->has_children() && (int)path.size() < D)
			{
				if (h->scheme() != w->scheme() || h->n_children() != w->n_children())
					break;
				int best = 0;
				for (int i = 1; i < w->n_children(); ++i)
					if (w->child(i).depth() > w->child(best).depth())
						best = i;
				V = child_vertices(V, h->scheme(), best);
				path.push_back(best);
				h = &h->child(best);
				w = &w->child(best);
			}
			const Eigen::RowVectorXd v = ref_vertices(dim).row(c.vertex);
			int vk = -1;
			for (int k = 0; k <= dim; ++k)
				if ((V.row(k) - v).norm() < 1e-14)
					vk = k;
			if (!h->has_children() && (int)path.size() == D && vk >= 0)
			{
				// Directions from the corner through the leaf's opposite face.
				std::vector<int> others;
				for (int k = 0; k <= dim; ++k)
					if (k != vk)
						others.push_back(k);
				auto dir_of = [&](const Eigen::RowVectorXd &s) {
					Eigen::RowVectorXd d = V.row(others[0]) - v;
					for (int j = 1; j < (int)others.size(); ++j)
						d += s(j - 1) * (V.row(others[j]) - V.row(others[0]));
					return Eigen::RowVectorXd(d / d.norm());
				};
				auto f_dir = [&](const Eigen::MatrixXd &S) {
					Eigen::MatrixXd Dm(S.rows(), dim);
					for (int i = 0; i < S.rows(); ++i)
						Dm.row(i) = dir_of(S.row(i));
					Eigen::VectorXd out = limit_det(id, Dm, xl);
					for (int i = 0; i < out.size(); ++i)
						if (!std::isfinite(out(i)))
							out(i) = std::numeric_limits<double>::infinity();
					return out;
				};
				const int N = 6, ns = dim - 1;
				std::vector<Eigen::RowVectorXd> grid;
				for (int i = 0; i <= N; ++i)
					for (int j = 0; (ns == 1 ? j == 0 : i + j <= N); ++j)
					{
						Eigen::RowVectorXd sv(ns);
						sv(0) = double(i) / N;
						if (ns == 2)
							sv(1) = double(j) / N;
						grid.push_back(sv);
					}
				Eigen::MatrixXd S(grid.size(), ns);
				for (size_t i = 0; i < grid.size(); ++i)
					S.row(i) = grid[i];
				const Eigen::VectorXd fs = f_dir(S);
				Eigen::Index ib = 0;
				for (Eigen::Index i = 0; i < fs.size(); ++i)
					if (fs(i) < fs(ib))
						ib = i;
				int its = 0;
				const auto [s_star, a] = simplex_minimize(f_dir, S.row(ib), 1e-4, 30, &its);
				if (std::isfinite(a))
				{
					const Eigen::RowVectorXd d = dir_of(s_star);
					auto it = deep_dirs_[id].find(path);
					if (it == deep_dirs_[id].end() || (it->second - d).norm() > 1e-12)
					{
						deep_dirs_[id][path] = d;
						changed = true;
					}
					logger().debug("Deep leaf: elem {} depth {} direction [{:.6g}, {:.6g}, {:.6g}] limit det F {:.4e} (newton its {}), {} directional nodes in the element",
								   id, D, d(0), d(1), dim > 2 ? d(2) : 0., a, its, deep_dirs_[id].size());
				}
			}
		}
		if (!changed)
			return false;
		rebuild_element_quadrature(id);
		if (deep_guard_x_.size() == n_bases_ * dim && !std::isfinite(element_energy(id, deep_guard_x_)))
		{
			logger().debug("Deep leaf: update of element {} makes its energy non-finite; reverted", id);
			quadrature_hierarchy_[id] = saved_tree;
			deep_dirs_[id] = saved_dirs;
			rebuild_element_quadrature(id);
			return false;
		}
		return true;
	}

	void ElasticForm::rebuild_element_quadrature(const int e) const
	{
		const int dim = is_volume_ ? 3 : 2;
		if (dir_nodes_.size() != bases_.size())
			dir_nodes_.resize(bases_.size());
		const bool has_dirs = e < (int)deep_dirs_.size() && !deep_dirs_[e].empty();
		if (!has_dirs)
		{
			update_quadrature(e, dim, quadrature_hierarchy_[e], quadrature_order_, bases_[e], geom_bases_[e], ass_vals_cache_, degenerate_ != nullptr);
			if (!dir_nodes_[e].empty() && ass_vals_cache_.is_initialized())
				ass_vals_cache_.mutable_entry(e).limit_node.clear();
			dir_nodes_[e].clear();
			return;
		}
		const auto &dirs = deep_dirs_[e];
		const Quadrature rule = degenerate_ ? degenerate_quadrature(dim) : corner_quadrature(dim, quadrature_order_);
		const double rule_sum = rule.weights.sum();
		const Eigen::RowVectorXd v = ref_vertices(dim).row(corner(e).vertex);

		std::vector<Eigen::RowVectorXd> P, cents;
		std::vector<double> W;
		std::vector<DirNode> dn;
		std::vector<int> path;
		std::function<void(const Tree &, const Eigen::MatrixXd &)> rec = [&](const Tree &t, const Eigen::MatrixXd &V) {
			if (t.has_children())
			{
				for (int i = 0; i < t.n_children(); ++i)
				{
					path.push_back(i);
					rec(t.child(i), child_vertices(V, t.scheme(), i));
					path.pop_back();
				}
				return;
			}
			Eigen::MatrixXd M(dim, dim);
			for (int j = 0; j < dim; ++j)
				M.col(j) = (V.row(j + 1) - V.row(0)).transpose();
			const double scale = std::abs(M.determinant()); // leaf volume / reference volume
			const auto it = dirs.find(path);
			if (it == dirs.end())
			{
				for (int q = 0; q < rule.size(); ++q)
				{
					const Eigen::RowVectorXd mu = rule.points.row(q);
					Eigen::RowVectorXd x = (1. - mu.sum()) * V.row(0);
					for (int j = 0; j < dim; ++j)
						x += mu(j) * V.row(j + 1);
					P.push_back(x);
					W.push_back(rule.weights(q) * scale);
				}
				return;
			}
			dn.push_back({int(P.size()), it->second, 0.});
			cents.push_back(V.colwise().mean());
			P.push_back(v + limit_r_ * it->second);
			W.push_back(rule_sum * scale);
		};
		rec(quadrature_hierarchy_[e], ref_vertices(dim));

		Quadrature quad;
		quad.points.resize(P.size(), dim);
		quad.weights.resize(P.size());
		for (size_t i = 0; i < P.size(); ++i)
		{
			quad.points.row(i) = P[i];
			quad.weights(i) = W[i];
		}
		bases_[e].set_quadrature([quad](Quadrature &q) { q = quad; });
		if (!ass_vals_cache_.is_initialized())
			log_and_throw_error("Deep-leaf quadrature needs an initialized assembly cache");
		ass_vals_cache_.update(e, is_volume_, bases_[e], geom_bases_[e]);

		// Mass of a directional node: |det grad X| at its leaf's centroid (the node itself sits at the corner).
		Eigen::MatrixXd C(cents.size(), dim);
		for (size_t k = 0; k < cents.size(); ++k)
			C.row(k) = cents[k];
		ElementAssemblyValues gv;
		gv.compute(e, is_volume_, C, bases_[e], geom_bases_[e]);
		for (size_t k = 0; k < dn.size(); ++k)
			dn[k].mass_det = std::abs(gv.det(k));

		Eigen::MatrixXd Dm(dn.size(), dim);
		for (size_t k = 0; k < dn.size(); ++k)
			Dm.row(k) = dn[k].dir;
		const std::vector<Eigen::MatrixXd> G = limit_grads(e, Dm);
		auto &lv = ass_vals_cache_.mutable_entry(e);
		lv.limit_node.assign(quad.points.rows(), 0);
		for (size_t k = 0; k < dn.size(); ++k)
		{
			const int qi = dn[k].q;
			lv.limit_node[qi] = 1;
			for (size_t i = 0; i < lv.basis_values.size(); ++i)
			{
				lv.basis_values[i].grad.row(qi) = G[k].row(i);
				lv.basis_values[i].grad_t_m.row(qi) = G[k].row(i);
			}
			lv.jac_it[qi] = Eigen::MatrixXd::Identity(dim, dim);
			lv.det(qi) = dn[k].mass_det;
		}
		dir_nodes_[e] = dn;
	}

	bool ElasticForm::commit_refinement(const int id, utils::Tree &subdivision_tree, const int max_depth) const
	{
		if (!refines_quadrature_)
			return false;
		if (experiments::settings().deep_leaf_depth > 0)
			return deep_commit(id, subdivision_tree);

		const int dim = is_volume_ ? 3 : 2;

		// Merge into quadrature_hierarchy_ and rebuild the live quadrature for
		// this element, if the tree is actually refined.
		if (!quadrature_hierarchy_[id].merge(subdivision_tree, max_depth))
			return false;
		auto &bs = bases_[id];
		auto &gbs = geom_bases_[id];
		update_quadrature(id, dim, quadrature_hierarchy_[id], quadrature_order_, bs, gbs, ass_vals_cache_, degenerate_ != nullptr);
		return true;
	}

	bool ElasticForm::direction_needs_recompute()
	{
		const bool r = recompute_;
		recompute_ = false;
		return r;
	}

	double ElasticForm::element_energy(const int e, const Eigen::VectorXd &x) const
	{
		const auto *nl = dynamic_cast<const NLAssembler *>(&assembler_);
		if (!nl)
			return 0; // linear assemblers are always finite
		ElementAssemblyValues vals;
		ass_vals_cache_.compute(e, is_volume_, bases_[e], geom_bases_[e], vals);
		const QuadratureVector da = vals.det.array() * vals.quadrature.weights.array();
		const Eigen::MatrixXd xm = x;
		const Eigen::MatrixXd xp = x_prev_.size() == x.size() ? Eigen::MatrixXd(x_prev_) : xm;
		return nl->compute_energy(NonLinearAssemblerData(vals, t_, dt_, xm, xp, da));
	}

	double ElasticForm::min_quadrature_det(const Eigen::VectorXd &x) const
	{
		const Eigen::VectorXd zero = Eigen::VectorXd::Zero(x.size());
		double m = std::numeric_limits<double>::infinity();
		for (int e = 0; e < (int)bases_.size(); ++e)
		{
			ElementAssemblyValues vals;
			ass_vals_cache_.compute(e, is_volume_, bases_[e], geom_bases_[e], vals);
			Eigen::VectorXd d = vals.eval_deformed_jacobian_determinant(x).cwiseQuotient(vals.eval_deformed_jacobian_determinant(zero));
			if (e < (int)dir_nodes_.size() && !dir_nodes_[e].empty())
			{
				const auto &dn = dir_nodes_[e];
				Eigen::MatrixXd Dm(dn.size(), is_volume_ ? 3 : 2);
				for (size_t k = 0; k < dn.size(); ++k)
					Dm.row(k) = dn[k].dir;
				const Eigen::VectorXd dd = limit_det(e, Dm, x);
				for (size_t k = 0; k < dn.size(); ++k)
					d(dn[k].q) = dd(k);
			}
			for (int i = 0; i < d.size(); ++i)
				if (std::isfinite(d(i)))
					m = std::min(m, d(i));
		}
		return m;
	}

	void ElasticForm::validate_rest() const
	{
		if (rest_validated_ || !is_conservative(check_inversion_))
			return;
		const int dim = is_volume_ ? 3 : 2;
		const Eigen::VectorXd x0 = Eigen::VectorXd::Zero(n_bases_ * dim);
		const auto [isvalid, id, tree] = is_valid(
			dim, bases_, geom_bases_, x0, jacobian_threshold_, conservative_max_iter_, &rest_nodes_, degenerate_.get());
		if (!isvalid)
			log_and_throw_error("Initial state has inverted elements (element {})!", id);
		rest_validated_ = true;
	}

	void ElasticForm::load_degenerate_edges(const std::string &path, const Eigen::VectorXi &input_to_node)
	{
		if (!is_conservative(check_inversion_))
			return;
		const int dim = is_volume_ ? 3 : 2;
		std::vector<utils::DegenerateCorner> corners = utils::read_degenerate_edges(path, input_to_node, bases_);
		if (corners.empty())
			return;

		degenerate_ = std::make_shared<const utils::DegenerateMesh>(utils::make_degenerate_mesh(dim, bases_, geom_bases_, std::move(corners)));

		std::vector<utils::ElementControlPoints> control_points(bases_.size());
		for (int e = 0; e < (int)bases_.size(); ++e)
			control_points[e] = utils::degenerate_control_points(*degenerate_, e);
		ass_vals_cache_.set_control_points(std::move(control_points));

		// All elements, marked or not, use the rule the degenerate checks
		// sample at: it has no corner, where det J is allowed to vanish.
		refines_quadrature_ = true;
		for (int e = 0; e < (int)bases_.size(); ++e)
		{
			quadrature_hierarchy_[e] = Tree();
			update_quadrature(e, dim, quadrature_hierarchy_[e], quadrature_order_, bases_[e], geom_bases_[e], ass_vals_cache_, true);
		}
		pending_refinement_.reset();
		rest_validated_ = false;

		int marked = 0;
		for (const auto &c : degenerate_->corners)
			marked += c.vertex >= 0;
		logger().info("Degenerate Jacobian checks: {} of {} elements have a degenerate corner", marked, bases_.size());
	}

	void ElasticForm::compute_cached_stiffness()
	{
		if (assembler_.is_linear() && cached_stiffness_.size() == 0)
		{
			assembler_.assemble(is_volume_, n_bases_, bases_, geom_bases_,
								ass_vals_cache_, t_, cached_stiffness_);
		}
	}
} // namespace polyfem::solver
