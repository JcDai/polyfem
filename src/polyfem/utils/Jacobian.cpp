#include <algorithm>
#include <cassert>
#include <cmath>
#include <limits>
#include <mutex>
#include <numeric>
#include <polyfem/utils/Logger.hpp>
#include <polyfem/utils/par_for.hpp>
#include <polyfem/utils/MaybeParallelFor.hpp>
#include "Jacobian.hpp"
#include <polyfem/autogen/auto_p_bases.hpp>
#include <polyfem/io/Evaluator.hpp>
#include <polyfem/quadrature/TriQuadrature.hpp>
#include <polyfem/quadrature/TetQuadrature.hpp>

#include <fstream>
#include <map>
#include <set>
#include <sstream>

#ifdef POLYFEM_WITH_MISO
#include "EigenAdapters.hpp"
#include "DegenerateAdapters.hpp"
#include <algorithms/solve.hpp>
#include <algorithms/minimize.hpp>
#include <algorithms/batch-minimize.hpp>
using namespace miso;
#endif

using namespace polyfem::assembler;

namespace polyfem::utils
{
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

	namespace
	{
		int control_point_order(const basis::ElementBases &basis, const basis::ElementBases &gbasis)
		{
			return std::max(basis.bases.front().order(), gbasis.bases.front().order());
		}

		int control_point_order(const std::vector<basis::ElementBases> &bases, const std::vector<basis::ElementBases> &gbases)
		{
			return control_point_order(bases[0], gbases[0]);
		}

		Eigen::MatrixXd control_point_nodes(const int dim, const int order)
		{
			Eigen::MatrixXd local_pts;
			if (dim == 3)
				autogen::p_nodes_3d(order, local_pts);
			else
				autogen::p_nodes_2d(order, local_pts);
			return local_pts;
		}

		/// Global node whose displacement each control point copies, or empty
		/// if the displacement basis is not nodal there. @p vals holds the
		/// element's bases evaluated at its @p n_per control points.
		std::vector<int> nodal_nodes(const ElementAssemblyValues &vals, const int n_per)
		{
			// A conforming Lagrange basis of the control-point order is 1 at
			// its own node and 0 at the others (up to the rounding of nodes
			// like 1/3), with a single global of weight 1: each control point
			// is then one entry of u, copied rather than computed.
			constexpr double tol = 1e-10;
			std::vector<int> owner(n_per, -1);
			bool nodal = true;
			for (int j = 0; j < (int)vals.basis_values.size(); ++j)
			{
				const auto &global = vals.basis_values[j].global;
				if (global.size() != 1 || global.front().val != 1)
					nodal = false;
				const Eigen::VectorXd &val = vals.basis_values[j].val;
				for (int k = 0; k < n_per; ++k)
				{
					if (std::abs(val(k) - 1) <= tol)
					{
						nodal = nodal && owner[k] < 0;
						owner[k] = j;
					}
					else if (std::abs(val(k)) > tol)
						nodal = false;
				}
			}
			nodal = nodal && std::find(owner.begin(), owner.end(), -1) == owner.end();
			if (!nodal)
				return {};

			std::vector<int> nodes(n_per);
			for (int k = 0; k < n_per; ++k)
				nodes[k] = vals.basis_values[owner[k]].global.front().index;
			return nodes;
		}

		/// Add the displacement at the control points to @p out, evaluating
		/// the basis there, which rounds: for geometry of higher order than the
		/// displacement, or a non-conforming basis.
		void evaluate_displacement(const ElementAssemblyValues &vals, const Eigen::Ref<const Eigen::VectorXd> &u, const int dim, Eigen::Ref<Eigen::MatrixXd> out)
		{
			for (std::size_t j = 0; j < vals.basis_values.size(); ++j)
				for (const auto &g : vals.basis_values[j].global)
					out += g.val * vals.basis_values[j].val * u.segment(g.index * dim, dim).transpose();
		}
	} // namespace

	Eigen::MatrixXd rest_nodes(const int dim, const std::vector<basis::ElementBases> &bases, const std::vector<basis::ElementBases> &gbases)
	{
		const Eigen::MatrixXd local_pts = control_point_nodes(dim, control_point_order(bases, gbases));
		const int n_per = local_pts.rows();
		Eigen::MatrixXd rest(bases.size() * n_per, dim);
		for (int e = 0; e < (int)bases.size(); ++e)
		{
			Eigen::MatrixXd mapped;
			gbases[e].eval_geom_mapping(local_pts, mapped);
			rest.middleRows(e * n_per, n_per) = mapped;
		}
		return rest;
	}

	Eigen::MatrixXd displacement_nodes(const int dim, const std::vector<basis::ElementBases> &bases, const std::vector<basis::ElementBases> &gbases, const Eigen::VectorXd &u, bool *exact)
	{
		const Eigen::MatrixXd local_pts = control_point_nodes(dim, control_point_order(bases, gbases));
		const int n_per = local_pts.rows();

		Eigen::MatrixXd disp = Eigen::MatrixXd::Zero(bases.size() * n_per, dim);
		bool all_exact = true;
		for (int e = 0; e < (int)bases.size(); ++e)
		{
			ElementAssemblyValues vals;
			vals.compute(e, dim == 3, local_pts, bases[e], gbases[e]);

			const std::vector<int> nodes = nodal_nodes(vals, n_per);
			if (!nodes.empty())
			{
				for (int k = 0; k < n_per; ++k)
					disp.row(e * n_per + k) = u.segment(nodes[k] * dim, dim).transpose();
				continue;
			}
			all_exact = false;
			evaluate_displacement(vals, u, dim, disp.middleRows(e * n_per, n_per));
		}
		if (exact)
			*exact = all_exact;
		return disp;
	}

	// The calls below are the ones rest_nodes() and displacement_nodes()
	// make, so the energy sees exactly the numbers the checks certify.
	ElementControlPoints element_control_points(const int dim, const basis::ElementBases &basis, const basis::ElementBases &gbasis)
	{
		ElementControlPoints cp;
		cp.order = control_point_order(basis, gbasis);
		const Eigen::MatrixXd local_pts = control_point_nodes(dim, cp.order);
		gbasis.eval_geom_mapping(local_pts, cp.rest);

		ElementAssemblyValues vals;
		vals.compute(0, dim == 3, local_pts, basis, gbasis);
		cp.nodes = nodal_nodes(vals, local_pts.rows());
		return cp;
	}

	std::vector<ElementControlPoints> element_control_points(const int dim, const std::vector<basis::ElementBases> &bases, const std::vector<basis::ElementBases> &gbases)
	{
		std::vector<ElementControlPoints> cps(bases.size());
		maybe_parallel_for(bases.size(), [&](int start, int end, int /*thread_id*/) {
			for (int e = start; e < end; ++e)
				cps[e] = element_control_points(dim, bases[e], gbases[e]);
		});
		return cps;
	}

	Eigen::MatrixXd control_point_displacement(const int dim, const ElementControlPoints &cp, const basis::ElementBases &basis, const basis::ElementBases &gbasis, const Eigen::Ref<const Eigen::VectorXd> &u)
	{
		const int n_per = cp.rest.rows();
		Eigen::MatrixXd disp(n_per, dim);
		if (!cp.nodes.empty())
		{
			for (int k = 0; k < n_per; ++k)
				disp.row(k) = u.segment(cp.nodes[k] * dim, dim).transpose();
			return disp;
		}
		disp.setZero();
		ElementAssemblyValues vals;
		vals.compute(0, dim == 3, control_point_nodes(dim, cp.order), basis, gbasis);
		evaluate_displacement(vals, u, dim, disp);
		return disp;
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

	Eigen::MatrixXd refined_nodes(const int dim, const int i)
	{
		Eigen::MatrixXd A(dim + 1, dim);
		if (dim == 2)
		{
			A << 0., 0.,
				1., 0.,
				0., 1.;
			switch (i)
			{
			case 0:
				break;
			case 1:
				A.col(0).array() += 1;
				break;
			case 2:
				A.col(1).array() += 1;
				break;
			case 3:
				A.array() -= 1;
				A *= -1;
				break;
			default:
				throw std::runtime_error("Invalid node index");
			}
		}
		else
		{
			A << 0, 0, 0,
				1, 0, 0,
				0, 1, 0,
				0, 0, 1;
			switch (i)
			{
			case 0:
				break;
			case 1:
				A.col(0).array() += 1;
				break;
			case 2:
				A.col(1).array() += 1;
				break;
			case 3:
				A.col(2).array() += 1;
				break;
			case 4:
			{
				Eigen::VectorXd tmp = 1. - A.col(0).array() - A.col(1).array();
				A.col(2) += A.col(1);
				A.col(1) = A.col(0);
				A.col(0) = tmp;
				break;
			}
			case 5:
			{
				Eigen::VectorXd tmp = 1. - A.col(0).array();
				A.col(1) += A.col(0);
				A.col(0) = tmp;
				break;
			}
			case 6:
			{
				Eigen::VectorXd tmp0 = A.col(0);
				Eigen::VectorXd tmp1 = A.col(1);
				A.col(0) = tmp1;
				A.col(1) = 1. - tmp0.array() - tmp1.array();
				A.col(2) += tmp0 + tmp1;
				break;
			}
			case 7:
			{
				Eigen::VectorXd tmp = A.col(1);
				A.col(0) += tmp;
				A.col(1) = 1. - tmp.array();
				A.col(2) += tmp;
				break;
			}
			default:
				throw std::runtime_error("Invalid node index");
			}
		}
		return A / 2;
	}

	Eigen::MatrixXd cone_nodes(const int dim, const int vertex, const int i)
	{
		// Same construction as pymiso's cone_subdivision(): the other vertices,
		// in increasing order, are the corners a, b, c of the facet, and the
		// pieces of its uniform split go to their rows in that order.
		if (dim != 3)
			log_and_throw_error("cone_nodes: only tetrahedra have a cone split, got dim {}", dim);
		if (vertex < 0 || vertex > 3 || i < 0 || i > 3)
			log_and_throw_error("cone_nodes: invalid vertex {} or child {}", vertex, i);
		Eigen::MatrixXd ref(4, 3);
		ref << 0, 0, 0,
			1, 0, 0,
			0, 1, 0,
			0, 0, 1;
		std::vector<int> slots;
		for (int k = 0; k < 4; ++k)
			if (k != vertex)
				slots.push_back(k);
		const Eigen::RowVector3d a = ref.row(slots[0]), b = ref.row(slots[1]), c = ref.row(slots[2]);
		const Eigen::RowVector3d ab = (a + b) / 2, ac = (a + c) / 2, bc = (b + c) / 2;
		const std::array<std::array<Eigen::RowVector3d, 3>, 4> pieces = {{
			{{a, ab, ac}},
			{{ab, b, bc}},
			{{ac, bc, c}},
			{{bc, ac, ab}},
		}};
		Eigen::MatrixXd out(4, 3);
		out.row(vertex) = ref.row(vertex);
		for (int k = 0; k < 3; ++k)
			out.row(slots[k]) = pieces[i][k];
		return out;
	}

	int n_children(const int dim, const int scheme)
	{
		if (subdivision::is_cone(scheme))
			return dim == 3 ? 4 : 2;
		return dim == 3 ? 8 : 4;
	}

	Eigen::MatrixXd child_nodes(const int dim, const int scheme, const int i)
	{
		if (subdivision::is_cone(scheme))
			return cone_nodes(dim, subdivision::cone_vertex(scheme), i);
		return refined_nodes(dim, i);
	}

	namespace
	{
		/// Leaves of @p tree in the simplex @p pts, and for each the sum of
		/// per-level exponents given by @p exponent(scheme).
		template <typename F>
		std::tuple<Eigen::MatrixXd, std::vector<int>> leaves(const Eigen::MatrixXd &pts, const Tree &tree, const F &exponent)
		{
			if (!tree.has_children())
				return {pts, std::vector<int>{0}};

			const int dim = pts.cols();
			Eigen::MatrixXd out;
			std::vector<int> levels;
			for (int i = 0; i < tree.n_children(); i++)
			{
				Eigen::MatrixXd uv;
				uv.setZero(dim + 1, dim + 1);
				uv.rightCols(dim) = child_nodes(dim, tree.scheme(), i);
				if (dim == 2)
					uv.col(0) = 1. - uv.col(2).array() - uv.col(1).array();
				else
					uv.col(0) = 1. - uv.col(3).array() - uv.col(1).array() - uv.col(2).array();

				Eigen::MatrixXd pts_ = uv * pts;

				auto [tmp, L] = leaves(pts_, tree.child(i), exponent);
				if (out.size() == 0)
					out = tmp;
				else
				{
					out.conservativeResize(out.rows() + tmp.rows(), Eigen::NoChange);
					out.bottomRows(tmp.rows()) = tmp;
				}
				const int e = exponent(tree.scheme());
				for (int &l : L)
					l += e;
				levels.insert(levels.end(), L.begin(), L.end());
			}
			return {out, levels};
		}
	} // namespace

	/// @brief given the position of the vertices of a triangle, extract the subtriangle vertices based on the tree
	/// @param pts vertices of the triangle
	/// @param tree refinement hiararchy
	/// @return vertices of the subtriangles, and the refinement level of each subtriangle
	std::tuple<Eigen::MatrixXd, std::vector<int>> extract_subelement(const Eigen::MatrixXd &pts, const Tree &tree)
	{
		return leaves(pts, tree, [](int) { return 1; });
	}

	std::tuple<Eigen::MatrixXd, std::vector<int>> extract_subelement_volumes(const Eigen::MatrixXd &pts, const Tree &tree)
	{
		const int dim = pts.cols();
		return leaves(pts, tree, [dim](int scheme) { return subdivision::is_cone(scheme) ? dim - 1 : dim; });
	}

	std::pair<Eigen::MatrixXd, Eigen::MatrixXd> fatten(const Eigen::MatrixXd &x, const int n)
	{
		constexpr double inf = std::numeric_limits<double>::infinity();
		Eigen::MatrixXd lo = x, hi = x;
		for (Eigen::Index i = 0; i < x.size(); ++i)
			for (int k = 0; k < n; ++k)
			{
				lo(i) = std::nextafter(lo(i), -inf);
				hi(i) = std::nextafter(hi(i), inf);
			}
		return {lo, hi};
	}

#ifdef POLYFEM_WITH_MISO
	namespace
	{
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

		/// @brief Build a refinement tree from a *spatial* subdivision path.
		///
		/// The static (*Val) problems split space only, with a single scheme,
		/// so each path entry is already the spatial child index -- unlike the
		/// space-time path handled by build_tree(). The child numbering matches
		/// the one refine_quadrature() uses in ElasticForm (checked for both
		/// triangles and tetrahedra, including vertex order, so it holds at
		/// every depth).
		void build_spatial_tree(Tree &tree, const std::vector<unsigned> &path, const unsigned n_children)
		{
			Tree *dst = &tree;
			for (const auto idx : path)
			{
				assert(idx < n_children);
				dst->add_children(n_children);
				dst = &(dst->child(idx));
			}
		}
		/// @brief RAII guard for miso's interval arithmetic FPU state.
		///
		/// RealInterval::init()/deinit() switch the FPU rounding mode to
		/// round-up and back to round-to-nearest. That is *per-thread* CPU
		/// state, so it must be paired on every exit path -- including the
		/// throws below, which previously left the whole thread stuck in
		/// round-up mode -- and re-established inside each worker when these
		/// routines are parallelized.
		struct FPUGuard
		{
			FPUGuard() { RealInterval::init(); }
			~FPUGuard() { RealInterval::deinit(); }
			FPUGuard(const FPUGuard &) = delete;
			FPUGuard &operator=(const FPUGuard &) = delete;
		};

		/// @brief Report an element type/order the conservative check cannot handle.
		///
		/// The miso-generated solvers only cover simplices: triangles up to
		/// P4 and tetrahedra up to P3. Anything else (quads, hexahedra,
		/// prisms, pyramids, or a higher order) lands here.
		[[noreturn]] void throw_unsupported(const int dim, const int order)
		{
			log_and_throw_error(
				"Conservative Jacobian check does not support {} of order {}; "
				"supported: triangles (P1-P4) and tetrahedra (P1-P3). Use "
				"\"check_inversion\": \"Discrete\" for this mesh.",
				dim == 2 ? "2D elements" : "3D elements", order);
		}
	} // anonymous namespace
#endif // POLYFEM_WITH_MISO

	namespace
	{
		/// Local indices, in polyfem's P3 node order, of the element's vertices
		/// and of the control point next to vertex v on the edge to vertex w.
		struct P3Layout
		{
			std::vector<int> vertex;
			std::vector<std::vector<int>> near;
		};

		P3Layout p3_layout(const int dim)
		{
			const Eigen::MatrixXd x = control_point_nodes(dim, 3);
			const auto find = [&](const Eigen::RowVectorXd &q) {
				for (int i = 0; i < x.rows(); ++i)
					if ((x.row(i) - q).cwiseAbs().maxCoeff() < 1e-12)
						return i;
				log_and_throw_error("P3 node layout: no node at the expected position");
			};
			Eigen::MatrixXd ref = Eigen::MatrixXd::Zero(dim + 1, dim);
			for (int k = 1; k <= dim; ++k)
				ref(k, k - 1) = 1;
			P3Layout layout;
			layout.vertex.resize(dim + 1);
			layout.near.assign(dim + 1, std::vector<int>(dim + 1, -1));
			for (int v = 0; v <= dim; ++v)
			{
				layout.vertex[v] = find(ref.row(v));
				for (int w = 0; w <= dim; ++w)
					if (w != v)
						layout.near[v][w] = find(ref.row(v) + (ref.row(w) - ref.row(v)) / 3.);
			}
			return layout;
		}

		int p3_dim(const std::vector<basis::ElementBases> &bases)
		{
			if (bases.empty())
				log_and_throw_error("Degenerate checks: empty mesh");
			const int n = bases.front().bases.size();
			if (n == 10)
				return 2;
			if (n == 20)
				return 3;
			log_and_throw_error("Degenerate checks require cubic triangles or tetrahedra");
		}

		/// The single global node of local basis @p i, which must have weight 1.
		int unconstrained_node(const basis::ElementBases &bs, const int i)
		{
			const auto &g = bs.bases[i].global();
			if (g.size() != 1 || g[0].val != 1)
				log_and_throw_error("Degenerate checks require unconstrained node numbering");
			return g[0].index;
		}
	} // namespace

	std::vector<DegenerateCorner> read_degenerate_edges(
		const std::string &path,
		const Eigen::VectorXi &input_to_node,
		const std::vector<basis::ElementBases> &bases)
	{
		if (path.empty())
			return {};
		std::ifstream file(path);
		if (!file)
			log_and_throw_error("Cannot read degenerate edges: {}", path);
		std::set<std::pair<int, int>> edges;
		std::string line;
		int number = 0;
		while (std::getline(file, line))
		{
			++number;
			if (line.find_first_not_of(" \t\r") == std::string::npos)
				continue;
			std::istringstream row(line);
			int a, b;
			std::string extra;
			if (!(row >> a >> b) || (row >> extra) || a < 0 || b < 0 || a == b
				|| a >= input_to_node.size() || b >= input_to_node.size())
				log_and_throw_error("Invalid degenerate edge at {}:{}", path, number);
			a = input_to_node[a];
			b = input_to_node[b];
			if (a < 0 || b < 0 || a == b)
				log_and_throw_error("Degenerate edge at {}:{} has no distinct PolyFEM node mapping", path, number);
			edges.emplace(a, b);
		}
		if (edges.empty())
			return {};

		const int dim = p3_dim(bases);
		const int n_per = dim == 3 ? 20 : 10;
		const int max_order = dim == 3 ? 2 : 1;
		const P3Layout layout = p3_layout(dim);

		std::set<std::pair<int, int>> found;
		std::vector<DegenerateCorner> result(bases.size());
		for (size_t e = 0; e < bases.size(); ++e)
		{
			const auto &bs = bases[e];
			if (int(bs.bases.size()) != n_per || bs.bases.front().order() != 3)
				log_and_throw_error("Degenerate edges require uniform cubic simplices");
			for (int v = 0; v <= dim; ++v)
			{
				for (int w = 0; w <= dim; ++w)
				{
					if (v == w)
						continue;
					const auto pair = std::make_pair(unconstrained_node(bs, layout.vertex[v]), unconstrained_node(bs, layout.near[v][w]));
					if (!edges.count(pair))
						continue;
					auto &c = result[e];
					if (c.vertex >= 0 && c.vertex != v)
						log_and_throw_error("Element {}: degenerate corners at more than one vertex are unsupported", e);
					c.vertex = v;
					c.near_nodes.push_back(layout.near[v][w]);
					found.insert(pair);
				}
			}
			if (int(result[e].near_nodes.size()) > max_order)
				log_and_throw_error("Element {}: {} collapsed control points at one corner; at most {} supported in {}D",
									e, result[e].near_nodes.size(), max_order, dim);
		}
		if (found != edges)
			log_and_throw_error("A degenerate edge is not a corner-to-adjacent-control-point edge of a cubic element");
		return result;
	}

#ifdef POLYFEM_WITH_MISO
	namespace
	{
		using miso::IntervalCoefficients;

		int factorial(const int n)
		{
			int v = 1;
			for (int i = 2; i <= n; ++i)
				v *= i;
			return v;
		}

		int multinomial(const std::vector<int> &a)
		{
			int n = 0, den = 1;
			for (const int v : a)
			{
				if (v < 0)
					return 0;
				n += v;
				den *= factorial(v);
			}
			return factorial(n) / den;
		}

		/// Barycentric multi-indices (lambda_0, x_1..x_dim) of the order-p nodes, in
		/// polyfem's node order.
		std::vector<std::vector<int>> lattice_indices(const int dim, const int p)
		{
			const Eigen::MatrixXd x = control_point_nodes(dim, p);
			std::vector<std::vector<int>> a(x.rows(), std::vector<int>(dim + 1));
			for (int i = 0; i < x.rows(); ++i)
			{
				a[i][0] = p;
				for (int d = 0; d < dim; ++d)
				{
					a[i][d + 1] = int(std::lround(p * x(i, d)));
					a[i][0] -= a[i][d + 1];
				}
			}
			return a;
		}

		using IntervalMatrix = std::vector<std::vector<Real>>;

		/// Exact Lagrange-to-Bernstein transform of order p in interval
		/// arithmetic (Gauss-Jordan on the Bernstein values at the nodes, which
		/// are rationals); no floating-point inverse is taken as exact.
		IntervalMatrix lagrange_to_bernstein(const int dim, const int p)
		{
			const auto a = lattice_indices(dim, p);
			const int n = a.size();
			IntervalMatrix b(n, std::vector<Real>(2 * n, Real(0.)));
			for (int i = 0; i < n; ++i)
			{
				for (int j = 0; j < n; ++j)
				{
					Real v(double(multinomial(a[j])));
					for (int d = 0; d <= dim; ++d)
						for (int k = 0; k < a[j][d]; ++k)
							v = v * Real(double(a[i][d])) / Real(double(p));
					b[i][j] = v;
				}
				b[i][n + i] = Real(1.);
			}
			for (int k = 0; k < n; ++k)
			{
				int pivot = k;
				for (int i = k + 1; i < n; ++i)
					if (std::abs(lower(b[i][k])) > std::abs(lower(b[pivot][k])))
						pivot = i;
				std::swap(b[k], b[pivot]);
				const Real divisor = b[k][k];
				if (lower(divisor) <= 0 && upper(divisor) >= 0)
					log_and_throw_error("Singular Lagrange-to-Bernstein conversion");
				for (int j = 0; j < 2 * n; ++j)
					b[k][j] = b[k][j] / divisor;
				for (int i = 0; i < n; ++i)
					if (i != k)
					{
						const Real f = b[i][k];
						for (int j = 0; j < 2 * n; ++j)
							b[i][j] = b[i][j] - f * b[k][j];
					}
			}
			IntervalMatrix result(n, std::vector<Real>(n));
			for (int i = 0; i < n; ++i)
				for (int j = 0; j < n; ++j)
					result[i][j] = b[i][n + j];
			return result;
		}

		/// Whether @p basis is Bernstein (true) or Lagrange (false) of order p.
		bool is_bernstein(const basis::ElementBases &basis, const int dim, const int p)
		{
			const Eigen::MatrixXd x = control_point_nodes(dim, p);
			std::vector<assembler::AssemblyValues> values;
			basis.evaluate_bases(x, values);
			bool bernstein = true, lagrange = true;
			for (int i = 0; i < x.rows(); ++i)
			{
				Eigen::MatrixXd expected;
				if (dim == 3)
					autogen::p_basis_value_3d(true, p, i, x, expected);
				else
					autogen::p_basis_value_2d(true, p, i, x, expected);
				bernstein = bernstein && values[i].val.isApprox(expected, 1e-12);
				Eigen::MatrixXd unit = Eigen::MatrixXd::Zero(x.rows(), 1);
				unit(i, 0) = 1;
				lagrange = lagrange && values[i].val.isApprox(unit, 1e-12);
			}
			if (!bernstein && !lagrange)
				log_and_throw_error("Unsupported basis in the degenerate Jacobian check: neither Bernstein nor Lagrange");
			return bernstein;
		}

		/// Bernstein coefficients of order 3 of the geometry of one element,
		/// exactly in interval arithmetic: Lagrange coefficients are converted,
		/// and lower orders elevated.
		std::vector<std::vector<Real>> rest_bernstein(const basis::ElementBases &gbs, const int dim, std::map<int, IntervalMatrix> &conversions)
		{
			const int q = gbs.bases.front().order();
			if (q > 3)
				log_and_throw_error("Degenerate checks: geometry of order {} > 3", q);
			const auto source = lattice_indices(dim, q);
			if (source.size() != gbs.bases.size())
				log_and_throw_error("Degenerate checks require simplex geometry");
			std::vector<std::vector<Real>> c(source.size(), std::vector<Real>(dim, Real(0.)));
			for (size_t i = 0; i < source.size(); ++i)
				for (const auto &g : gbs.bases[i].global())
					for (int d = 0; d < dim; ++d)
						c[i][d] = c[i][d] + Real(g.val) * Real(g.node[d]);
			if (!is_bernstein(gbs, dim, q))
			{
				if (!conversions.count(q))
					conversions.emplace(q, lagrange_to_bernstein(dim, q));
				const auto &l2b = conversions.at(q);
				std::vector<std::vector<Real>> converted(c.size(), std::vector<Real>(dim, Real(0.)));
				for (size_t i = 0; i < c.size(); ++i)
					for (size_t j = 0; j < c.size(); ++j)
						for (int d = 0; d < dim; ++d)
							converted[i][d] = converted[i][d] + l2b[i][j] * c[j][d];
				c = std::move(converted);
			}
			const auto target = lattice_indices(dim, 3);
			std::vector<std::vector<Real>> out(target.size(), std::vector<Real>(dim, Real(0.)));
			for (size_t i = 0; i < target.size(); ++i)
				for (size_t j = 0; j < source.size(); ++j)
				{
					std::vector<int> delta(dim + 1);
					for (int d = 0; d <= dim; ++d)
						delta[d] = target[i][d] - source[j][d];
					const int factor = multinomial(delta);
					if (!factor)
						continue;
					const Real weight = Real(double(multinomial(source[j]) * factor)) / Real(double(multinomial(target[i])));
					for (int d = 0; d < dim; ++d)
						out[i][d] = out[i][d] + weight * c[j][d];
				}
			return out;
		}

		/// Displacement of every control point of @p m: entries of @p u, copied.
		Eigen::MatrixXd degenerate_displacement(const DegenerateMesh &m, const Eigen::VectorXd &u)
		{
			Eigen::MatrixXd d(m.dofs.size(), m.dim);
			for (size_t i = 0; i < m.dofs.size(); ++i)
				d.row(i) = u.segment(m.dofs[i] * m.dim, m.dim).transpose();
			return d;
		}

		/// p = rest + displacement, with the displacement in [d_lo, d_hi].
		IntervalCoefficients degenerate_coefficients(const DegenerateMesh &m, const Eigen::MatrixXd &d_lo, const Eigen::MatrixXd &d_hi)
		{
			IntervalCoefficients p(m.rest_lo.rows(), m.dim);
			for (Eigen::Index i = 0; i < m.rest_lo.rows(); ++i)
				for (int c = 0; c < m.dim; ++c)
					p(i, c) = Real(m.rest_lo(i, c), m.rest_hi(i, c)) + Real(d_lo(i, c), d_hi(i, c));
			return p;
		}

		/// a + b == c + d exactly, from error-free sums (TwoSum): (fl(x+y), x+y-fl(x+y))
		/// represents x+y exactly and uniquely. Round-to-nearest only.
		bool same_sum(const double a, const double b, const double c, const double d)
		{
			const auto two_sum = [](const double x, const double y) {
				const volatile double s = x + y;
				const volatile double yy = s - x;
				const volatile double xx = s - yy;
				return std::make_pair(double(s), (x - xx) + (y - yy));
			};
			return two_sum(a, b) == two_sum(c, d);
		}

		/// Require the corner's collapse to be exact in configuration @p d: each
		/// marked control point and the corner have the same Bernstein
		/// coefficient rest + displacement, as real numbers. The degenerate
		/// state treats the corresponding coefficients of det J as exact zeros,
		/// so equality within rounding is not enough. Call in round-to-nearest
		/// (before the interval guard).
		void require_collapse(const DegenerateMesh &m, const int e, const Eigen::MatrixXd &d)
		{
			const auto &c = m.corners[e];
			const int o = e * m.n_per;
			const int v = o + c.vertex;
			for (const int near : c.near_nodes)
			{
				const int n = o + near;
				for (int k = 0; k < m.dim; ++k)
				{
					bool same;
					if (m.rest_lo(n, k) == m.rest_hi(n, k) && m.rest_lo(v, k) == m.rest_hi(v, k))
						same = same_sum(m.rest_lo(n, k), d(n, k), m.rest_lo(v, k), d(v, k));
					else // rest only known as an enclosure: it must be the same one
						same = m.rest_lo(n, k) == m.rest_lo(v, k) && m.rest_hi(n, k) == m.rest_hi(v, k) && d(n, k) == d(v, k);
					if (!same)
						log_and_throw_error("Element {}: the control point marked degenerate at local vertex {} does not coincide with it", e, c.vertex);
				}
			}
		}

		template <typename Problem>
		Problem mark(Problem problem, const DegenerateMesh &m, const int e)
		{
			const auto &c = m.corners[e];
			if (c.vertex < 0)
				return problem;
			const unsigned dv = m.dim == 3 ? miso::tet_domain_vertex[c.vertex] : miso::tri_domain_vertex[c.vertex];
			// Static: the corner. Continuous: the corner times the time interval,
			// i.e. the domain vertices (corner, t=0) and (corner, t=1).
			const unsigned mask = Problem::hasObjective ? (3u << (2 * dv)) : (1u << dv);
			problem.degenerateState = Problem::degenerateStateFromVertices(mask, unsigned(c.near_nodes.size()));
			return problem;
		}

		/// What child q (miso's global child index) of the generated degenerate
		/// classes is, following the scheme layout in generate.py
		/// (DegenerateChecks): a time-only split, or child @p child of a
		/// spatial split with Tree scheme @p scheme.
		struct DegenerateChild
		{
			bool time_only = false;
			int scheme = subdivision::Uniform;
			int child = 0;
		};

		static_assert(miso::P3TriDegVal::schemes.size() == 1 && miso::P3TriDegVal::schemes[0] == 4);
		static_assert(miso::P3TetDegVal::schemes.size() == 5 && miso::P3TetDegVal::schemes[0] == 8 && miso::P3TetDegVal::schemes[1] == 4);
		static_assert(miso::P3TriDegCGV::schemes.size() == 3 && miso::P3TriDegCGV::schemes[0] == 8 && miso::P3TriDegCGV::schemes[1] == 2 && miso::P3TriDegCGV::schemes[2] == 4);
		static_assert(miso::P3TetDegCGV::schemes.size() == 7 && miso::P3TetDegCGV::schemes[0] == 16 && miso::P3TetDegCGV::schemes[1] == 2
					  && miso::P3TetDegCGV::schemes[2] == 4 && miso::P3TetDegCGV::schemes[6] == 8);

		DegenerateChild degenerate_child(const int dim, const bool continuous, const unsigned q)
		{
			if (!continuous)
			{
				if (dim == 2 || q < 8)
					return {false, subdivision::Uniform, int(q)};
				return {false, subdivision::cone(int(q - 8) / 4), int(q - 8) % 4};
			}
			if (dim == 2)
			{
				if (q < 8)
					return {false, subdivision::Uniform, int(q / 2)};
				if (q < 10)
					return {true};
				return {false, subdivision::Uniform, int(q - 10)};
			}
			if (q < 16)
				return {false, subdivision::Uniform, int(q / 2)};
			if (q < 18)
				return {true};
			if (q < 34)
				return {false, subdivision::cone(int(q - 18) / 4), int(q - 18) % 4};
			return {false, subdivision::Uniform, int(q - 34)};
		}

		void build_degenerate_tree(Tree &tree, const std::vector<unsigned> &path, const int dim, const bool continuous)
		{
			Tree *dst = &tree;
			for (const auto q : path)
			{
				const DegenerateChild c = degenerate_child(dim, continuous, q);
				if (c.time_only)
					continue;
				dst->add_children(n_children(dim, c.scheme), c.scheme);
				dst = &(dst->child(c.child));
			}
		}
	} // namespace
#endif

	DegenerateMesh make_degenerate_mesh(
		const int dim,
		const std::vector<basis::ElementBases> &bases,
		const std::vector<basis::ElementBases> &gbases,
		std::vector<DegenerateCorner> corners)
	{
		DegenerateMesh m;
#ifdef POLYFEM_WITH_MISO
		if (p3_dim(bases) != dim)
			log_and_throw_error("Degenerate checks: {}D mesh with elements of another dimension", dim);
		m.dim = dim;
		m.n_per = dim == 3 ? 20 : 10;
		if (corners.empty())
			corners.resize(bases.size());
		if (corners.size() != bases.size())
			log_and_throw_error("Degenerate checks: {} corner entries for {} elements", corners.size(), bases.size());
		m.corners = std::move(corners);
		m.rest_lo.resize(bases.size() * m.n_per, dim);
		m.rest_hi.resize(bases.size() * m.n_per, dim);
		m.dofs.resize(bases.size() * m.n_per);

		const FPUGuard fpu_guard;
		std::map<int, IntervalMatrix> conversions;
		for (size_t e = 0; e < bases.size(); ++e)
		{
			const auto &bs = bases[e];
			if (int(bs.bases.size()) != m.n_per || bs.bases.front().order() != 3 || !is_bernstein(bs, dim, 3))
				log_and_throw_error("Degenerate checks require a cubic Bernstein displacement (element {})", e);
			for (int i = 0; i < m.n_per; ++i)
				m.dofs[e * m.n_per + i] = unconstrained_node(bs, i);
			const auto rest = rest_bernstein(gbases[e], dim, conversions);
			for (int i = 0; i < m.n_per; ++i)
				for (int d = 0; d < dim; ++d)
				{
					m.rest_lo(e * m.n_per + i, d) = lower(rest[i][d]);
					m.rest_hi(e * m.n_per + i, d) = upper(rest[i][d]);
				}
		}
#else
		log_and_throw_error("Enable the Miso library to allow the degenerate Jacobian check!");
#endif
		return m;
	}

	quadrature::Quadrature degenerate_quadrature(const int dim)
	{
		quadrature::Quadrature quad;
#ifdef POLYFEM_WITH_MISO
		if (dim == 2)
			quadrature::TriQuadrature(false).get_quadrature(miso::degenerate_quadrature_order, quad);
		else
			quadrature::TetQuadrature(false).get_quadrature(miso::degenerate_quadrature_order, quad);
		const int n = dim == 2 ? miso::degenerate_tri_rule_size : miso::degenerate_tet_rule_size;
		bool same = quad.points.rows() == n && quad.points.cols() == dim;
		for (int i = 0; same && i < n; ++i)
			for (int d = 0; d < dim; ++d)
				same = same && quad.points(i, d) == (dim == 2 ? miso::degenerate_tri_rule[i][d] : miso::degenerate_tet_rule[i][d]);
		if (!same)
			log_and_throw_error("The quadrature rule of order {} is not the one the degenerate checks were generated for; regenerate them (src/polyfem/miso/generate.py)",
								miso::degenerate_quadrature_order);
		if (quad.weights.minCoeff() <= 0)
			log_and_throw_error("The degenerate checks' quadrature rule has non-positive weights");
#else
		log_and_throw_error("Enable the Miso library to allow the degenerate Jacobian check!");
#endif
		return quad;
	}

	ElementControlPoints degenerate_control_points(const DegenerateMesh &m, const int e)
	{
		ElementControlPoints cp;
		cp.order = 3;
		cp.bernstein = true;
		cp.rest = m.rest_lo.middleRows(e * m.n_per, m.n_per);
		cp.rest_hi = m.rest_hi.middleRows(e * m.n_per, m.n_per);
		cp.nodes.assign(m.dofs.begin() + e * m.n_per, m.dofs.begin() + (e + 1) * m.n_per);
		return cp;
	}

	Eigen::VectorXd robust_evaluate_jacobian_bernstein(
		const Eigen::MatrixXd &rest_lo,
		const Eigen::MatrixXd &rest_hi,
		const Eigen::MatrixXd &disp,
		const Eigen::MatrixXd &uv)
	{
		Eigen::VectorXd result(uv.rows());
#ifdef POLYFEM_WITH_MISO
		const int dim = rest_lo.cols();
		const FPUGuard fpu_guard;
		IntervalCoefficients p(rest_lo.rows(), dim);
		for (Eigen::Index i = 0; i < rest_lo.rows(); ++i)
			for (int c = 0; c < dim; ++c)
				p(i, c) = Real(rest_lo(i, c), rest_hi(i, c)) + Real(disp(i, c));
		for (int k = 0; k < uv.rows(); ++k)
		{
			const Real j = dim == 2 ? miso::eval_p3tri_jac_bernstein(p, 0, uv, k) : miso::eval_p3tet_jac_bernstein(p, 0, uv, k);
			const double lo = lower(j), hi = upper(j);
			result(k) = hi > 0 ? (std::max(lo, 0.) + hi) / 2 : std::numeric_limits<double>::quiet_NaN();
		}
#else
		log_and_throw_error("Enable the Miso library to allow robust Jacobian evaluation!");
#endif
		return result;
	}

#ifdef POLYFEM_WITH_MISO
	namespace
	{
		/// The static degenerate check on every element; calls @p on_element
		/// with (element, invalid, witness path) and stops when it returns false.
		template <typename F>
		void degenerate_static(const DegenerateMesh &m, const Eigen::VectorXd &u, const double threshold, const unsigned max_iter, F &&on_element)
		{
			const Eigen::MatrixXd d = degenerate_displacement(m, u);
			const int n_elem = m.corners.size();
			for (int e = 0; e < n_elem; ++e)
				if (m.corners[e].vertex >= 0)
					require_collapse(m, e, d);
			const FPUGuard fpu_guard;
			const IntervalCoefficients p = degenerate_coefficients(m, d, d);

			auto run = [&](auto problem, const int e) {
				Parameters params;
				params.constraintEpsilon.assign(1, threshold);
				params.maxIter = max_iter;
				params.findOne = true;
				Info info;
				auto sols = solve(std::move(problem), params, &info);
				if (!info.success())
				{
					logger().warn("Degenerate Jacobian solve gave up at element {} after {} iterations", e, info.numIterations);
					return on_element(e, true, std::vector<unsigned>{});
				}
				if (sols.empty())
					return on_element(e, false, std::vector<unsigned>{});
				return on_element(e, true, sols.front().history ? sols.front().history->path() : std::vector<unsigned>{});
			};

			for (int e = 0; e < n_elem; ++e)
			{
				const int o = e * m.n_per;
				const bool go_on = m.dim == 2 ? run(mark(miso::make_p3tri_degval(p, o), m, e), e)
											  : run(mark(miso::make_p3tet_degval(p, o), m, e), e);
				if (!go_on)
					return;
			}
		}
	} // namespace
#endif

	Eigen::VectorXd robust_evaluate_jacobian(
		const int order,
		const Eigen::MatrixXd &rest,
		const Eigen::MatrixXd &disp,
		const Eigen::MatrixXd &uv)
	{
		Eigen::VectorXd result(uv.rows());
#ifdef POLYFEM_WITH_MISO
		const int dim = rest.cols();
		const FPUGuard fpu_guard;

		// Where the check passed, the true value is positive: keep the part
		// of the interval above zero and take its midpoint. Computed in
		// round-up, so it cannot underflow to zero and stays within (0, hi].
		const auto clip = [](const Real &j) {
			const double lo = lower(j), hi = upper(j);
			if (!(hi > 0))
				return std::numeric_limits<double>::quiet_NaN();
			return (std::max(lo, 0.) + hi) / 2;
		};
		const auto at_points = [&](const auto &jac) {
			for (int k = 0; k < uv.rows(); ++k)
				result(k) = clip(jac(uv, k));
		};

		if (dim == 2)
		{
			switch (order)
			{
			case 1:
				result.setConstant(clip(eval_p1tri_jac(rest, disp, 0)));
				break;
			case 2:
				at_points(make_p2tri_jac(rest, disp, 0));
				break;
			case 3:
				at_points(make_p3tri_jac(rest, disp, 0));
				break;
			case 4:
				at_points(make_p4tri_jac(rest, disp, 0));
				break;
			default:
				throw_unsupported(dim, order);
			}
		}
		else
		{
			switch (order)
			{
			case 1:
				result.setConstant(clip(eval_p1tet_jac(rest, disp, 0)));
				break;
			case 2:
				at_points(make_p2tet_jac(rest, disp, 0));
				break;
			case 3:
				at_points(make_p3tet_jac(rest, disp, 0));
				break;
			default:
				throw_unsupported(dim, order);
			}
		}
#else
		log_and_throw_error("Enable the Miso library to allow robust Jacobian evaluation!");
#endif
		return result;
	}

	// Debug utility: counts elements with invalid Jacobian in a given configuration.
	// Not used in the solve loop; called for diagnostics and visualization.
	std::vector<int> count_invalid(
		const int dim,
		const std::vector<basis::ElementBases> &bases,
		const std::vector<basis::ElementBases> &gbases,
		const Eigen::VectorXd &u,
		const unsigned max_iter,
		const Eigen::MatrixXd *rest,
		const DegenerateMesh *degenerate)
	{
		std::vector<int> invalidList;
#ifdef POLYFEM_WITH_MISO
		if (degenerate && !degenerate->empty())
		{
			degenerate_static(*degenerate, u, 0., max_iter, [&](const int e, const bool invalid, const std::vector<unsigned> &) {
				if (invalid)
					invalidList.push_back(e);
				return true;
			});
			return invalidList;
		}
		const int order = control_point_order(bases, gbases);
		const int n_per = std::max(bases[0].bases.size(), gbases[0].bases.size());
		const int n_elem = static_cast<int>(bases.size());
		// Rest and displacement go to miso separately, and are read before
		// the interval guard switches the FPU to round-up, so a rest computed
		// here matches the cached one.
		const Eigen::MatrixXd rest_here = rest ? Eigen::MatrixXd() : rest_nodes(dim, bases, gbases);
		const Eigen::MatrixXd &r = rest ? *rest : rest_here;
		const Eigen::MatrixXd d = displacement_nodes(dim, bases, gbases, u);
		const FPUGuard fpu_guard;

		auto run_solve = [&](auto problem, int e) {
			Parameters params;
			params.constraintEpsilon.assign(1, 0.0);
			params.maxIter = max_iter;
			params.findOne = true;
			Info info;
			auto sols = solve(std::move(problem), params, &info);
			if (!info.success())
				logger().warn("Jacobian solve gave up at element {} after {} iterations", e, info.numIterations);
			return !sols.empty();
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
					invalid = eval_p1tri_jac(r, d, o) <= 0;
					break;
				case 2:
					invalid = run_solve(make_p2tri_val(r, d, o), e);
					break;
				case 3:
					invalid = run_solve(make_p3tri_val(r, d, o), e);
					break;
				case 4:
					invalid = run_solve(make_p4tri_val(r, d, o), e);
					break;
				default:
					throw_unsupported(dim, order);
				}
			}
			else
			{
				switch (order)
				{
				case 1:
					invalid = eval_p1tet_jac(r, d, o) <= 0;
					break;
				case 2:
					invalid = run_solve(make_p2tet_val(r, d, o), e);
					break;
				case 3:
					invalid = run_solve(make_p3tet_val(r, d, o), e);
					break;
				default:
					throw_unsupported(dim, order);
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
		const Eigen::MatrixXd *rest,
		const DegenerateMesh *degenerate)
	{
#ifdef POLYFEM_WITH_MISO
		if (degenerate && !degenerate->empty())
		{
			int bad = -1;
			Tree witness;
			degenerate_static(*degenerate, u, threshold, max_iter, [&](const int e, const bool invalid, const std::vector<unsigned> &path) {
				if (!invalid)
					return true;
				bad = e;
				build_degenerate_tree(witness, path, dim, false);
				return false;
			});
			if (bad >= 0)
				return {false, bad, std::move(witness)};
			return {true, -1, Tree{}};
		}
		const int order = control_point_order(bases, gbases);
		const int n_per = std::max(bases[0].bases.size(), gbases[0].bases.size());
		const int n_elem = static_cast<int>(bases.size());
		// Rest and displacement go to miso separately, and are read before
		// the interval guard switches the FPU to round-up, so a rest computed
		// here matches the cached one.
		const Eigen::MatrixXd rest_here = rest ? Eigen::MatrixXd() : rest_nodes(dim, bases, gbases);
		const Eigen::MatrixXd &r = rest ? *rest : rest_here;
		const Eigen::MatrixXd d = displacement_nodes(dim, bases, gbases, u);
		const FPUGuard fpu_guard;

		// Spatial split: 2^dim children per level.
		const unsigned n_children = (dim == 2) ? 4u : 8u;
		Tree witness;

		// The witness subdomain solve() returns carries its own subdivision
		// history, so the location of the violation comes for free; it is
		// only a matter of not discarding it.
		auto run_solve = [&](auto problem, int e) {
			Parameters params;
			params.constraintEpsilon.assign(1, threshold);
			params.maxIter = max_iter;
			params.findOne = true;
			Info info;
			auto sols = solve(std::move(problem), params, &info);
			if (!info.success())
			{
				// Ambiguous counts as invalid, but there is no certified
				// location to refine towards, so leave the tree empty.
				logger().warn("Jacobian solve gave up at element {} after {} iterations", e, info.numIterations);
				return true;
			}
			if (sols.empty())
				return false;
			if (sols.front().history)
				build_spatial_tree(witness, sols.front().history->path(), n_children);
			return true;
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
					invalid = eval_p1tri_jac(r, d, o) <= 0;
					break;
				case 2:
					invalid = run_solve(make_p2tri_val(r, d, o), e);
					break;
				case 3:
					invalid = run_solve(make_p3tri_val(r, d, o), e);
					break;
				case 4:
					invalid = run_solve(make_p4tri_val(r, d, o), e);
					break;
				default:
					throw_unsupported(dim, order);
				}
			}
			else
			{
				switch (order)
				{
				case 1:
					invalid = eval_p1tet_jac(r, d, o) <= 0;
					break;
				case 2:
					invalid = run_solve(make_p2tet_val(r, d, o), e);
					break;
				case 3:
					invalid = run_solve(make_p3tet_val(r, d, o), e);
					break;
				default:
					throw_unsupported(dim, order);
				}
			}
			// P1 has a constant Jacobian per element, so there is nothing to
			// localize and the tree stays empty, as for the continuous check.
			if (invalid)
				return {false, e, std::move(witness)};
		}
		return {true, -1, Tree{}};
#else
		log_and_throw_error("Enable Bezier or Miso library to allow robust Jacobian check!");
		return {false, -1, Tree{}};
#endif
	}

#ifdef POLYFEM_WITH_MISO
	namespace
	{
		std::tuple<double, int, double, Tree> degenerate_max_time_step(
			const DegenerateMesh &m,
			const Eigen::VectorXd &u1,
			const Eigen::VectorXd &u2,
			const double precision,
			const double threshold,
			const unsigned max_iter)
		{
			const int n_elem = m.corners.size();
			const Eigen::MatrixXd d1 = degenerate_displacement(m, u1);
			const Eigen::MatrixXd d2 = degenerate_displacement(m, u2);
			// The collapse holds over the whole step: at both ends, hence (the
			// map being linear in t) everywhere in between.
			for (int e = 0; e < n_elem; ++e)
				if (m.corners[e].vertex >= 0)
				{
					require_collapse(m, e, d1);
					require_collapse(m, e, d2);
				}
			// Same widening as max_time_step() (hocgv-miso fix.pdf, steps 2-3).
			const auto [d1_lo, d1_hi] = fatten(d1, 1);
			const auto [d2_lo, d2_hi] = fatten(d2, 2);
			const FPUGuard fpu_guard;
			const IntervalCoefficients p0 = degenerate_coefficients(m, d1_lo, d1_hi);
			const IntervalCoefficients p1 = degenerate_coefficients(m, d2_lo, d2_hi);

			double step = 1.0;
			int invalid_id = -1;
			double invalid_step = 1.0;
			Tree tree;

			auto run_batch = [&](auto factory) {
				using ProblemType = decltype(factory(0));
				std::vector<ProblemType> problems;
				problems.reserve(n_elem);
				for (int e = 0; e < n_elem; ++e)
					problems.push_back(mark(factory(e * m.n_per), m, e));

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
						logger().error("Degenerate Jacobian batch_minimize gave up with step size 0 after {} iterations", info.numIterations);
					else
						logger().warn("Degenerate Jacobian batch_minimize gave up after {} iterations (step={})", info.numIterations, t_lo);
				}
				if (t_lo < step)
				{
					step = t_lo;
					invalid_id = info.pathToFeasibleId ? static_cast<int>(*info.pathToFeasibleId) : -1;
					invalid_step = upper(result);
					tree = Tree{};
					build_degenerate_tree(tree, info.pathToFeasible, m.dim, true);
				}
			};

			if (m.dim == 2)
				run_batch([&](int o) { return miso::make_p3tri_degcgv(p0, p1, o); });
			else
				run_batch([&](int o) { return miso::make_p3tet_degcgv(p0, p1, o); });

			return {step, invalid_id, invalid_step, tree};
		}
	} // namespace
#endif

	std::tuple<double, int, double, Tree> max_time_step(
		const int dim,
		const std::vector<basis::ElementBases> &bases,
		const std::vector<basis::ElementBases> &gbases,
		const Eigen::VectorXd &u1,
		const Eigen::VectorXd &u2,
		double precision,
		double threshold,
		const unsigned max_iter,
		const Eigen::MatrixXd *rest,
		const DegenerateMesh *degenerate)
	{
#ifdef POLYFEM_WITH_MISO
		if (degenerate && !degenerate->empty())
			return degenerate_max_time_step(*degenerate, u1, u2, precision, threshold, max_iter);
		const int order = control_point_order(bases, gbases);
		const int n_per = std::max(bases[0].bases.size(), gbases[0].bases.size());
		const int n_elem = static_cast<int>(bases.size());
		// As in is_valid(): rest and displacement separately, before the guard.
		const Eigen::MatrixXd rest_here = rest ? Eigen::MatrixXd() : rest_nodes(dim, bases, gbases);
		const Eigen::MatrixXd &r = rest ? *rest : rest_here;
		bool exact = true;
		const Eigen::MatrixXd d1_pt = displacement_nodes(dim, bases, gbases, u1, &exact);
		const Eigen::MatrixXd d2_pt = displacement_nodes(dim, bases, gbases, u2);
		if (!exact)
		{
			static std::once_flag warned;
			std::call_once(warned, [] {
				logger().warn("Continuous Jacobian check: the displacement is evaluated at the control points rather than "
							  "read from the solver's vector (geometry of higher order than the displacement, or a "
							  "non-conforming basis), so the certificate does not strictly cover the rounding of the step");
			});
		}

#ifndef MISO_FP_ARITHMETIC
		// Erratum (hocgv-miso fix.pdf), steps 2-3, on the solver's own vector:
		// u2 = u1 + T dx was computed with one rounding (step_along in
		// polysolve), so one representable value on each side contains the
		// exact endpoint, and one more on both endpoints covers any correctly
		// rounded point in between, which is what the solver takes once the
		// step is capped. The rest positions are exact data for the problem.
		const auto [d1_lo, d1_hi] = fatten(d1_pt, 1);
		const auto [d2_lo, d2_hi] = fatten(d2_pt, 2);
		const Box d1{d1_lo, d1_hi}, d2{d2_lo, d2_hi};
#else
		const Eigen::MatrixXd &d1 = d1_pt, &d2 = d2_pt;
#endif
		const FPUGuard fpu_guard;

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
				invalid_id = info.pathToFeasibleId ? static_cast<int>(*info.pathToFeasibleId) : -1;
				invalid_step = upper(result);
				tree = Tree{};
				// P1 is a problem in t only: its subdivisions carry no
				// spatial location to refine towards.
				if (order > 1)
					build_tree(tree, info.pathToFeasible, n_children);
			}
		};

		if (dim == 2)
		{
			switch (order)
			{
			case 1:
				run_batch([&](int o) { return make_p1tri_cgv(r, d1, d2, o); });
				break;
			case 2:
				run_batch([&](int o) { return make_p2tri_cgv(r, d1, d2, o); });
				break;
			case 3:
				run_batch([&](int o) { return make_p3tri_cgv(r, d1, d2, o); });
				break;
			case 4:
				run_batch([&](int o) { return make_p4tri_cgv(r, d1, d2, o); });
				break;
			default:
				throw_unsupported(dim, order);
			}
		}
		else
		{
			switch (order)
			{
			case 1:
				run_batch([&](int o) { return make_p1tet_cgv(r, d1, d2, o); });
				break;
			case 2:
				run_batch([&](int o) { return make_p2tet_cgv(r, d1, d2, o); });
				break;
			case 3:
				run_batch([&](int o) { return make_p3tet_cgv(r, d1, d2, o); });
				break;
			default:
				throw_unsupported(dim, order);
			}
		}

		return {step, invalid_id, invalid_step, tree};
#else
		log_and_throw_error("Enable Bezier or Miso library to allow robust Jacobian check!");
		return {1.0, -1, 1.0, Tree{}};
#endif
	}
} // namespace polyfem::utils
