#include <numeric>
#include <cstdlib>
#include <fstream>
#include <set>
#include <map>
#include <polyfem/utils/Logger.hpp>
#include <polyfem/utils/par_for.hpp>
#include "Jacobian.hpp"
#include <polyfem/autogen/auto_p_bases.hpp>
#include <polyfem/io/Evaluator.hpp>

#ifdef POLYFEM_WITH_MISO
#include "EigenAdapters.hpp"
#include "MisoBezier.hpp"
#include <algorithms/solve.hpp>
#include <algorithms/minimize.hpp>
#include <algorithms/batch-minimize.hpp>
using namespace miso;
#endif

using namespace polyfem::assembler;

namespace polyfem::utils
{
    DegenerateTets read_degenerate_edges(const std::string &path,
        const Eigen::VectorXi &input_to_node,
        const std::vector<basis::ElementBases> &bases)
    {
        if (path.empty()) return {};
        std::ifstream file(path);
        if (!file) throw std::runtime_error("Cannot read degenerate edges: " + path);
        std::set<std::pair<int, int>> edges;
        std::string line;
        int number = 0;
        while (std::getline(file, line)) {
            ++number;
            if (line.find_first_not_of(" \t\r") == std::string::npos) continue;
            std::istringstream row(line);
            int a, b;
            std::string extra;
            if (!(row >> a >> b) || (row >> extra) || a < 0 || b < 0 || a == b ||
                a >= input_to_node.size() || b >= input_to_node.size())
                throw std::runtime_error("Invalid degenerate edge at " + path + ":" + std::to_string(number));
            a = input_to_node[a]; b = input_to_node[b];
            if (a < 0 || b < 0 || a == b)
                throw std::runtime_error("Degenerate edge has no distinct PolyFEM node mapping");
            edges.emplace(a, b);
        }
        if (edges.empty()) return {};
        std::set<std::pair<int, int>> found;
        DegenerateTets result(bases.size());
        constexpr int near[4][4] = {{-1,4,9,10}, {5,-1,6,12}, {8,7,-1,14}, {11,13,15,-1}};
        for (size_t e = 0; e < bases.size(); ++e) {
            const auto &bs = bases[e].bases;
            if (bs.size() != 20 || bs.front().order() != 3)
                throw std::runtime_error("Degenerate edges require uniform cubic tetrahedra");
            auto id = [&](int i) {
                const auto &g = bs[i].global();
                if (g.size() != 1 || g[0].val != 1)
                    throw std::runtime_error("Degenerate edges require unconstrained node numbering");
                return g[0].index;
            };
            for (int v = 0; v < 4; ++v) {
                for (int w = 0; w < 4; ++w) {
                    if (v == w) continue;
                    const auto pair = std::make_pair(id(v), id(near[v][w]));
                    if (!edges.count(pair)) continue;
                    auto &d = result[e];
                    if (d.vertex >= 0 && d.vertex != v)
                        throw std::runtime_error("Multiple isolated degenerate corners in one tet are unsupported");
                    d.vertex = v;
                    d.near_nodes.push_back(near[v][w]);
                    found.insert(pair);
                }
            }
            if (result[e].near_nodes.size() > 2)
                throw std::runtime_error("Three collapsed directions require unsupported order-3 degeneracy");
        }
        if (found != edges)
            throw std::runtime_error("Degenerate edge is not a cone-to-near-control-point edge of a cubic tet");
        return result;
    }

	Eigen::MatrixXd extract_nodes(const int dim, const std::vector<basis::ElementBases> &bases, const std::vector<basis::ElementBases> &gbases, const Eigen::VectorXd &u, int order, int n_elem)
	{
		if (n_elem < 0)
			n_elem = bases.size();
		Eigen::MatrixXd local_pts;
		if (dim == 3)
			autogen::p_nodes_3d(order, local_pts);
		else
			autogen::p_nodes_2d(order, local_pts);
		const int n_basis_per_cell = local_pts.rows();
		Eigen::MatrixXd cp = Eigen::MatrixXd::Zero(n_elem * n_basis_per_cell, dim);
		for (int e = 0; e < n_elem; ++e)
		{
			ElementAssemblyValues vals;
			vals.compute(e, dim == 3, local_pts, bases[e], gbases[e]);

			for (std::size_t j = 0; j < vals.basis_values.size(); ++j)
				for (const auto &g : vals.basis_values[j].global)
					cp.middleRows(e * n_basis_per_cell, n_basis_per_cell) += g.val * vals.basis_values[j].val * u.segment(g.index * dim, dim).transpose();

			Eigen::MatrixXd mapped;
			gbases[e].eval_geom_mapping(local_pts, mapped);
			cp.middleRows(e * n_basis_per_cell, n_basis_per_cell) += mapped;
		}
		return cp;
	}

	Eigen::MatrixXd extract_nodes(const int dim, const basis::ElementBases &basis, const basis::ElementBases &gbasis, const Eigen::VectorXd &u, int order)
	{
		Eigen::MatrixXd local_pts;
		if (dim == 3)
			autogen::p_nodes_3d(order, local_pts);
		else
			autogen::p_nodes_2d(order, local_pts);

		Eigen::MatrixXd cp;
		gbasis.eval_geom_mapping(local_pts, cp);

		ElementAssemblyValues vals;
		vals.compute(0, dim == 3, local_pts, basis, gbasis);
		for (std::size_t j = 0; j < vals.basis_values.size(); ++j)
			for (const auto &g : vals.basis_values[j].global)
				cp += g.val * vals.basis_values[j].val * u.segment(g.index * dim, dim).transpose();

		return cp;
	}

	Eigen::VectorXd robust_evaluate_jacobian(
		const int order,
		const Eigen::MatrixXd &cp,
		const Eigen::MatrixXd &uv)
	{
#ifdef POLYFEM_WITH_MISO
		const int dim = cp.cols();
		Eigen::VectorXd result(uv.rows());

		// TODO: replace with constant function evaluator (same as used for P=1 static validity)
		std::vector<Eigen::MatrixXd> grads(cp.rows(), Eigen::MatrixXd::Zero(uv.rows(), dim));
		for (int bid = 0; bid < cp.rows(); bid++)
		{
			if (dim == 2)
				autogen::p_grad_basis_value_2d(false, order, bid, uv, grads[bid]);
			else
				autogen::p_grad_basis_value_3d(false, order, bid, uv, grads[bid]);
		}
		for (int k = 0; k < uv.rows(); k++)
		{
			Eigen::MatrixXd jac_mat = Eigen::MatrixXd::Zero(dim, dim);
			for (int bid = 0; bid < cp.rows(); bid++)
				jac_mat += cp.row(bid).transpose() * grads[bid].row(k);
			result(k) = jac_mat.determinant();
		}
		return result;
#else
		log_and_throw_error("Enable Bezier or Miso library to allow robust Jacobian evaluation!");
		return Eigen::VectorXd::Zero(uv.rows());
#endif
	}

#ifdef POLYFEM_WITH_MISO
	namespace
	{
		// Diagnostic only: MISO expects Bernstein coefficients, whereas the old
		// extract_nodes API returns interpolation-point samples.
		miso_detail::BezierData extract_check_data(int dim,
			const std::vector<basis::ElementBases> &bases,
			const std::vector<basis::ElementBases> &gbases,
			const Eigen::VectorXd &u, int order)
		{
			static const bool use_samples = [] {
				const char *mode = std::getenv("POLYFEM_EXPERIMENT_EXTRACT_NODES");
				const bool enabled = mode && std::string(mode) == "1";
				if (enabled)
					logger().warn("EXPERIMENT: conservative checks use extract_nodes samples as MISO coefficients");
				return enabled;
			}();
			if (!use_samples)
				return miso_detail::extract_bezier(dim, bases, gbases, u, order);
			const Eigen::MatrixXd samples = extract_nodes(dim, bases, gbases, u, order, -1);
			miso_detail::BezierData result{miso_detail::Matrix(samples.rows(), std::vector<miso::Real>(dim))};
			for (int i = 0; i < samples.rows(); ++i)
				for (int d = 0; d < dim; ++d)
					result.rows[i][d] = miso::Real(samples(i, d));
			return result;
		}

		void build_tree(Tree &tree, const std::vector<unsigned> &path, unsigned n_spatial)
		{
			// Space-time child i maps to spatial child i/2 (even=lower t, odd=upper t).
			// Children beyond 2*n_spatial are time-only subdivisions and are skipped.
			Tree *dst = &tree;
			for (const auto idx : path)
			{
				if (idx < 2 * n_spatial)
				{
					dst->add_children(n_spatial);
					dst = &(dst->child(idx / 2));
				}
				// else: time-only subdivision, skip
			}
		}
	} // anonymous namespace
#endif // POLYFEM_WITH_MISO

	// Debug utility: counts elements with invalid Jacobian in a given configuration.
	// Not used in the solve loop; called for diagnostics and visualization.
	std::vector<int> count_invalid(
		const int dim,
		const std::vector<basis::ElementBases> &bases,
		const std::vector<basis::ElementBases> &gbases,
		const Eigen::VectorXd &u,
		const unsigned max_iter,
        const DegenerateTets &degenerate)
	{
		std::vector<int> invalidList;
#ifdef POLYFEM_WITH_MISO
		miso_detail::IntervalScope interval_scope;
		const int order = std::max(bases[0].bases.front().order(), gbases[0].bases.front().order());
		const int n_per = std::max(bases[0].bases.size(), gbases[0].bases.size());
		const auto cp = extract_check_data(dim, bases, gbases, u, order);
		const int n_elem = static_cast<int>(bases.size());

		auto run_solve = [&](auto problem, int e) {
			Parameters params;
			params.constraintEpsilon.assign(1, 0.0);
			params.maxIter = max_iter;
			params.findOne = true;
			Info info;
			auto sols = solve(std::move(problem), params, &info);
			if (!info.success())
				logger().warn("Jacobian solve gave up at element {} after {} iterations", e, info.numIterations);
			return !info.success() || !sols.empty();
		};

		for (int e = 0; e < n_elem; ++e)
		{
			const int o = e * n_per;
			bool invalid = false;
			if (dim == 2)
			{
				switch (order)
				{
				case 1:
					invalid = JacEval_P1Tri(rv<3>(cp, o, 0, tri1_perm), rv<3>(cp, o, 1, tri1_perm)) <= 0;
					break;
				case 2:
					invalid = run_solve(make_p2tri_val(cp, o), e);
					break;
				case 3:
					invalid = run_solve(make_p3tri_val(cp, o), e);
					break;
				case 4:
					invalid = run_solve(make_p4tri_val(cp, o), e);
					break;
				default:
					throw std::invalid_argument("Order not supported");
				}
			}
			else
			{
				switch (order)
				{
				case 1:
					invalid = JacEval_P1Tet(rv<4>(cp, o, 0, tet1_perm), rv<4>(cp, o, 1, tet1_perm), rv<4>(cp, o, 2, tet1_perm)) <= 0;
					break;
				case 2:
					invalid = run_solve(make_p2tet_val(cp, o), e);
					break;
				case 3:
					invalid = run_solve(miso_detail::mark(make_p3tet_val(cp, o), cp, nullptr, o, e, degenerate), e);
					break;
				default:
					throw std::invalid_argument("Order not supported");
				}
			}
			if (invalid)
				invalidList.push_back(e);
		}
#else
		log_and_throw_error("Enable Bezier or Miso library to allow robust Jacobian check!");
#endif
		return invalidList;
	}

	std::tuple<bool, int, Tree>
	is_valid(
		const int dim,
		const std::vector<basis::ElementBases> &bases,
		const std::vector<basis::ElementBases> &gbases,
		const Eigen::VectorXd &u,
		const double threshold,
		const unsigned max_iter,
        const DegenerateTets &degenerate)
	{
#ifdef POLYFEM_WITH_MISO
		miso_detail::IntervalScope interval_scope;
		const int order = std::max(bases[0].bases.front().order(), gbases[0].bases.front().order());
		const int n_per = std::max(bases[0].bases.size(), gbases[0].bases.size());
		const auto cp = extract_check_data(dim, bases, gbases, u, order);
		const int n_elem = static_cast<int>(bases.size());

		auto run_solve = [threshold, max_iter](auto problem, int e) {
			Parameters params;
			params.constraintEpsilon.assign(1, threshold);
			params.maxIter = max_iter;
			params.findOne = true;
			Info info;
			auto sols = solve(std::move(problem), params, &info);
			if (!info.success())
			{
				logger().warn("Jacobian solve gave up at element {} after {} iterations", e, info.numIterations);
				return true; // ambiguous counts as invalid
			}
			return !info.success() || !sols.empty();
		};

		for (int e = 0; e < n_elem; ++e)
		{
			const int o = e * n_per;
			bool invalid = false;
			if (dim == 2)
			{
				switch (order)
				{
				case 1:
					invalid = JacEval_P1Tri(rv<3>(cp, o, 0, tri1_perm), rv<3>(cp, o, 1, tri1_perm)) <= 0;
					break;
				case 2:
					invalid = run_solve(make_p2tri_val(cp, o), e);
					break;
				case 3:
					invalid = run_solve(make_p3tri_val(cp, o), e);
					break;
				case 4:
					invalid = run_solve(make_p4tri_val(cp, o), e);
					break;
				default:
					throw std::invalid_argument("Order not supported");
				}
			}
			else
			{
				switch (order)
				{
				case 1:
					invalid = JacEval_P1Tet(rv<4>(cp, o, 0, tet1_perm), rv<4>(cp, o, 1, tet1_perm), rv<4>(cp, o, 2, tet1_perm)) <= 0;
					break;
				case 2:
					invalid = run_solve(make_p2tet_val(cp, o), e);
					break;
				case 3:
					invalid = run_solve(miso_detail::mark(make_p3tet_val(cp, o), cp, nullptr, o, e, degenerate), e);
					break;
				default:
					throw std::invalid_argument("Order not supported");
				}
			}
			if (invalid)
			{
				return {false, e, Tree{}};
			}
		}
		return {true, -1, Tree{}};
#else
		log_and_throw_error("Enable Bezier or Miso library to allow robust Jacobian check!");
		return {false, -1, Tree{}};
#endif
	}

	std::tuple<double, int, double, Tree> max_time_step(
		const int dim,
		const std::vector<basis::ElementBases> &bases,
		const std::vector<basis::ElementBases> &gbases,
		const Eigen::VectorXd &u1,
		const Eigen::VectorXd &u2,
		double precision,
		double threshold,
		const unsigned max_iter,
        const DegenerateTets &degenerate)
	{
#ifdef POLYFEM_WITH_MISO
		miso_detail::IntervalScope interval_scope;
		const int order = std::max(bases[0].bases.front().order(), gbases[0].bases.front().order());
		const int n_per = std::max(bases[0].bases.size(), gbases[0].bases.size());
		const auto cp1 = extract_check_data(dim, bases, gbases, u1, order);
		const auto cp2 = extract_check_data(dim, bases, gbases, u2, order);
		const int n_elem = static_cast<int>(bases.size());

		double step = 1.0;
		int invalid_id = -1;
		double invalid_step = 1.0;
		Tree tree;

		// Spatial split: 2^dim children (ignore time subdivisions)
		const unsigned n_children = (dim == 2) ? 4u : 8u;

		// Build a vector of all per-element problems (same type for all elements) and
		// run a single batch_minimize over the entire mesh.
		auto run_batch = [&](auto factory) {
			using ProblemType = decltype(factory(0));
			std::vector<ProblemType> problems;
			problems.reserve(n_elem);
			for (int e = 0; e < n_elem; ++e)
				problems.push_back(factory(e * n_per));

			Parameters params;
			params.targetPrecision = precision;
			params.constraintEpsilon.assign(1, threshold);
			params.maxIter = max_iter;
			params.requiredMinimum = 0.;
			params.minBoxWidth = 0.;
			Info info;
			auto result = batch_minimize(std::move(problems), params, &info);
			const double t_lo = lower(result);
			if (!info.success())
			{
				if (t_lo <= 0)
					logger().error("Jacobian batch_minimize gave up with step size 0 after {} iterations", info.numIterations);
				else
					logger().warn("Jacobian batch_minimize gave up after {} iterations (step={})", info.numIterations, t_lo);
			}
			if (t_lo < step)
			{
				step = t_lo;
				invalid_id = static_cast<int>(info.pathToFeasibleId);
				invalid_step = upper(result);
				tree = Tree{};
				build_tree(tree, info.pathToFeasible, n_children);
			}
		};

		if (dim == 2)
		{
			switch (order)
			{
			case 1:
				run_batch([&](int o) { return make_p1tri_cgv(cp1, cp2, o); });
				break;
			case 2:
				run_batch([&](int o) { return make_p2tri_cgv(cp1, cp2, o); });
				break;
			case 3:
				run_batch([&](int o) { return make_p3tri_cgv(cp1, cp2, o); });
				break;
			case 4:
				run_batch([&](int o) { return make_p4tri_cgv(cp1, cp2, o); });
				break;
			default:
				throw std::invalid_argument("Order not supported");
			}
		}
		else
		{
			switch (order)
			{
			case 1:
				run_batch([&](int o) { return make_p1tet_cgv(cp1, cp2, o); });
				break;
			case 2:
				run_batch([&](int o) { return make_p2tet_cgv(cp1, cp2, o); });
				break;
			case 3:
				run_batch([&](int o) { return miso_detail::mark(make_p3tet_cgv(cp1, cp2, o), cp1, &cp2, o, o / n_per, degenerate); });
				break;
			default:
				throw std::invalid_argument("Order not supported");
			}
		}

		return {step, invalid_id, invalid_step, tree};
#else
		log_and_throw_error("Enable Bezier or Miso library to allow robust Jacobian check!");
		return {1.0, -1, 1.0, Tree{}};
#endif
	}
} // namespace polyfem::utils
