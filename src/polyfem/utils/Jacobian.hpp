#pragma once

#include <polyfem/basis/ElementBases.hpp>
#include <polyfem/quadrature/Quadrature.hpp>
#include <string>
#include <sstream>
#include <tuple>
#include <utility>
#include <vector>
#include <spdlog/fmt/fmt.h>

namespace polyfem::utils
{
	/// @brief How a node of a refinement Tree is split into its children.
	///
	/// Uniform is the regular 1:2^dim split (refined_nodes()). Cone(v), in 3D
	/// only, splits the facet opposite polyfem vertex v uniformly and cones
	/// each of its 4 pieces to v (cone_nodes()), as the degenerate checks do
	/// for an order-2 corner.
	namespace subdivision
	{
		constexpr int Uniform = 0;
		constexpr int cone(const int vertex) { return 1 + vertex; }
		constexpr bool is_cone(const int scheme) { return scheme > 0; }
		constexpr int cone_vertex(const int scheme) { return scheme - 1; }
	} // namespace subdivision

	class Tree
	{
	public:
		Tree() {}
		Tree(const Tree &T)
		{
			scheme_ = T.scheme_;
			if (T.has_children())
			{
				for (int i = 0; i < T.n_children(); i++)
					this->children.push_back(std::make_unique<Tree>(T.child(i)));
			}
		}
		Tree operator=(const Tree &T)
		{
			if (this != &T)
			{
				this->children.clear();
				scheme_ = T.scheme_;
				if (T.has_children())
				{
					for (int i = 0; i < T.n_children(); i++)
						this->children.push_back(std::make_unique<Tree>(T.child(i)));
				}
			}
			return *this;
		}
		/// A node already split one way keeps that split: a branch of @p T
		/// split differently there is not merged below it.
		bool merge(const Tree &T, int max_depth = 2)
		{
			bool flag = false;
			if (!T.has_children() || max_depth <= 0)
				return flag;
			if (!this->has_children())
			{
				this->add_children(T.n_children(), T.scheme());
				flag = true;
				max_depth--;
			}
			else if (scheme_ != T.scheme())
				return flag;
			for (int i = 0; i < T.n_children(); i++)
				flag = this->child(i).merge(T.child(i), max_depth) || flag;
			return flag;
		}

		// Debug print
		friend std::ostream &operator<<(
			std::ostream &ost, const Tree &T)
		{
			ost << "(";
			if (T.has_children())
			{
				for (int i = 0; i < T.n_children(); i++)
					ost << T.child(i) << ", ";
			}
			ost << ")";
			return ost;
		}

		bool has_children() const { return !children.empty(); }
		int n_children() const { return children.size(); }
		int depth() const
		{
			if (!has_children())
				return 0;
			int d = 0;
			for (int i = 0; i < n_children(); i++)
				d = std::max(d, child(i).depth());
			return d + 1;
		}
		int n_leaves() const
		{
			if (!has_children())
				return 1;
			int n = 0;
			for (int i = 0; i < n_children(); i++)
				n += child(i).n_leaves();
			return n;
		}
		Tree &child(int i) { return *children[i]; }
		const Tree &child(int i) const { return *children[i]; }
		void add_children(int n, const int scheme = subdivision::Uniform)
		{
			scheme_ = scheme;
			for (int i = 0; i < n; i++)
				children.push_back(std::make_unique<Tree>());
		}
		/// @brief How this node is split into its children (see subdivision).
		int scheme() const { return scheme_; }

	private:
		std::vector<std::unique_ptr<Tree>> children;
		int scheme_ = subdivision::Uniform;
	};

	/// @brief Vertices of child @p i of the reference simplex under the
	/// regular 1:2^dim split used for quadrature refinement, in the
	/// parent's reference coordinates. Rows are vertices.
	Eigen::MatrixXd refined_nodes(const int dim, const int i);

	/// @brief Vertices of child @p i of the reference tetrahedron under the
	/// cone split at polyfem vertex @p vertex (subdivision::cone): the facet
	/// opposite the vertex is split uniformly and each of its 4 pieces coned to
	/// it. The vertex keeps its row in every child. Rows are vertices.
	Eigen::MatrixXd cone_nodes(const int dim, const int vertex, const int i);

	/// @brief Children of the reference simplex under @p scheme (see subdivision).
	int n_children(const int dim, const int scheme);
	Eigen::MatrixXd child_nodes(const int dim, const int scheme, const int i);

	/// @brief Vertices of every leaf simplex of @p tree, obtained by
	/// recursively applying child_nodes() to the simplex @p pts.
	/// @param pts vertices of the root simplex, one per row
	/// @param tree refinement hierarchy
	/// @return stacked leaf vertices ((dim+1) rows per leaf) and the depth of each leaf
	std::tuple<Eigen::MatrixXd, std::vector<int>> extract_subelement(const Eigen::MatrixXd &pts, const Tree &tree);

	/// @brief As extract_subelement(), but returning the volume of each leaf
	/// relative to the root as a power of two (a uniform split divides it by
	/// 2^dim, a cone split by 2^(dim-1)): leaf k has volume 2^-exponents[k].
	std::tuple<Eigen::MatrixXd, std::vector<int>> extract_subelement_volumes(const Eigen::MatrixXd &pts, const Tree &tree);

	/// @brief An element allowed a zero of det J at one corner: the edge
	/// control points listed in @p near_nodes (local indices, one per incident
	/// edge, next to the corner) coincide with the corner's control point, so
	/// det J vanishes at the corner to order near_nodes.size(). vertex < 0
	/// means none.
	struct DegenerateCorner
	{
		int vertex = -1;
		std::vector<int> near_nodes;
	};

	/// @brief Read degenerate corners from a file of directed edges, one pair
	/// "corner adjacent" of zero-based input mesh point rows per line: the
	/// corner, and the control point next to it on an incident edge that
	/// collapses onto it. Elements must be cubic simplices (P3 triangles: one
	/// collapsed control point per corner; P3 tetrahedra: one or two) with
	/// unconstrained node numbering, and every listed edge must belong to
	/// some element.
	/// @param input_to_node input mesh point row to polyfem node
	/// @return one entry per element
	std::vector<DegenerateCorner> read_degenerate_edges(
		const std::string &path,
		const Eigen::VectorXi &input_to_node,
		const std::vector<basis::ElementBases> &bases);

	/// @brief Data for the degenerate conservative checks.
	///
	/// When these are given, every element is checked with the degenerate
	/// solvers (P3, Bernstein control points, the corner of a marked element
	/// excluded through its degenerate state, det J sampled at the points of
	/// polyfem's interior quadrature rule of order
	/// miso::degenerate_quadrature_order), whether marked or not, so that all
	/// elements are treated the same way.
	struct DegenerateMesh
	{
		int dim = 0;
		/// Control points per element (10 or 20).
		int n_per = 0;
		/// One per element.
		std::vector<DegenerateCorner> corners;
		/// Bernstein coefficients of the rest geometry, of order 3, as
		/// enclosures [rest_lo, rest_hi]: row e * n_per + i is local control
		/// point i of element e, in polyfem's node order.
		Eigen::MatrixXd rest_lo, rest_hi;
		/// Displacement dof of each of those control points: the global node
		/// whose displacement is that Bernstein coefficient.
		std::vector<int> dofs;

		bool empty() const { return corners.empty(); }
	};

	/// @brief Validate the discretization for the degenerate checks and build
	/// their data. The displacement must be a Bernstein basis of order 3 with
	/// one global node of weight 1 per local basis function; the geometry
	/// may be Lagrange or Bernstein of order at most 3 (its Bernstein
	/// coefficients are computed in interval arithmetic and degree-elevated).
	/// Declared corners are TRUSTED exact geometric identities, not checked:
	/// each adjacent control receives the corner's interval after conversion.
	/// Incorrect declarations change the geometry that is certified.
	DegenerateMesh make_degenerate_mesh(
		const int dim,
		const std::vector<basis::ElementBases> &bases,
		const std::vector<basis::ElementBases> &gbases,
		std::vector<DegenerateCorner> corners);

	/// @brief The quadrature rule the degenerate checks sample at, as
	/// polyfem builds it, after checking that it is the one they were
	/// generated with.
	quadrature::Quadrature degenerate_quadrature(const int dim);

	/// @brief One element's control points, as the conservative checks see
	/// them: the nodes of order max(displacement, geometry).
	struct ElementControlPoints
	{
		int order = 0;
		/// Rest positions, one row per control point (same values as the
		/// element's rows of rest_nodes()).
		Eigen::MatrixXd rest;
		/// Global node whose displacement each control point copies, when the
		/// displacement basis is nodal there (as in displacement_nodes());
		/// empty when the displacement has to be evaluated instead.
		std::vector<int> nodes;
		/// Set for the degenerate checks (DegenerateMesh): the control points
		/// are Bernstein coefficients, rest is the lower and rest_hi the upper
		/// end of their enclosure, and nodes their displacement dofs.
		bool bernstein = false;
		Eigen::MatrixXd rest_hi;
	};

	ElementControlPoints element_control_points(const int dim, const basis::ElementBases &basis, const basis::ElementBases &gbasis);

	/// @brief Control points of every element, to compute once and reuse.
	std::vector<ElementControlPoints> element_control_points(const int dim, const std::vector<basis::ElementBases> &bases, const std::vector<basis::ElementBases> &gbases);

	/// @brief Displacement of one element's control points: the same numbers
	/// displacement_nodes() gives the checks.
	Eigen::MatrixXd control_point_displacement(const int dim, const ElementControlPoints &cp, const basis::ElementBases &basis, const basis::ElementBases &gbasis, const Eigen::Ref<const Eigen::VectorXd> &u);

	/// @brief Jacobian determinant of one element's map at reference points
	/// @p uv, for the elastic energy under a conservative check.
	///
	/// It is evaluated with the generated expression the checks use (JacEval),
	/// in interval arithmetic, on the same inputs: the rest positions and the
	/// displacements of the control points, separately. The interval therefore
	/// provably contains the true value, and if the check accepted this
	/// configuration the true value is positive, so the part of the interval
	/// below zero is discarded: each entry is the midpoint of [max(lo, 0), hi],
	/// positive whenever hi > 0 (log J stays finite wherever the check passed),
	/// and NaN when hi <= 0 (certainly inverted there).
	/// @param rest Rest positions of the control points (ElementControlPoints::rest).
	/// @param disp Their displacements (control_point_displacement()).
	Eigen::VectorXd robust_evaluate_jacobian(
		const int order,
		const Eigen::MatrixXd &rest,
		const Eigen::MatrixXd &disp,
		const Eigen::MatrixXd &uv);

	/// @brief robust_evaluate_jacobian() for the degenerate checks: the map is
	/// given by Bernstein coefficients of order 3, rest in [rest_lo, rest_hi]
	/// plus @p disp, and det J is evaluated with the expression those checks
	/// use (JacEval_P3*Bernstein).
	Eigen::VectorXd robust_evaluate_jacobian_bernstein(
		const Eigen::MatrixXd &rest_lo,
		const Eigen::MatrixXd &rest_hi,
		const Eigen::MatrixXd &disp,
		const Eigen::MatrixXd &uv);

	/// @brief Control points of a DegenerateMesh element, for the energy.
	ElementControlPoints degenerate_control_points(const DegenerateMesh &mesh, const int e);

	/// @param rest Rest positions from rest_nodes(); computed here if null.
	std::vector<int> count_invalid(
		const int dim,
		const std::vector<basis::ElementBases> &bases,
		const std::vector<basis::ElementBases> &gbases,
		const Eigen::VectorXd &u,
		const unsigned max_iter = 1'000'000,
		const Eigen::MatrixXd *rest = nullptr,
		const DegenerateMesh *degenerate = nullptr);

	/// @brief Certify that no element of configuration @p u is inverted.
	/// @return (valid, id of the first invalid element or -1, witness tree).
	///         The tree is the subdivision path to where the violation was
	///         found inside that element, in the same child numbering as the
	///         quadrature refinement; it is empty for P1 (constant Jacobian,
	///         nothing to localize), when the violation is already visible at
	///         the element's corners, or when the solver gave up.
	/// @param rest Rest positions from rest_nodes(); computed here if null.
	/// @param degenerate If given, use the degenerate checks (DegenerateMesh)
	///        for every element; @p rest is then unused.
	std::tuple<bool, int, Tree> is_valid(
		const int dim,
		const std::vector<basis::ElementBases> &bases,
		const std::vector<basis::ElementBases> &gbases,
		const Eigen::VectorXd &u,
		const double threshold = 0,
		const unsigned max_iter = 1'000'000,
		const Eigen::MatrixXd *rest = nullptr,
		const DegenerateMesh *degenerate = nullptr);

	/// @brief Rest positions of every element's control points: the nodes of
	/// order max(displacement, geometry), mapped by the geometry. Compute it
	/// once (in round-to-nearest) and pass it to the checks below, so they all
	/// see the same numbers, which are also the ones the energy sees through
	/// element_control_points().
	Eigen::MatrixXd rest_nodes(const int dim, const std::vector<basis::ElementBases> &bases, const std::vector<basis::ElementBases> &gbases);

	/// @brief Displacement of every element's control points. When the
	/// displacement basis is nodal at those points (conforming, of the
	/// control-point order) each is an entry of @p u, copied; otherwise it is
	/// evaluated there, which rounds. @p exact reports which.
	Eigen::MatrixXd displacement_nodes(const int dim, const std::vector<basis::ElementBases> &bases, const std::vector<basis::ElementBases> &gbases, const Eigen::VectorXd &u, bool *exact = nullptr);

	/// @brief [lo, hi] with each entry of @p x widened by @p n representable
	/// values on each side (hocgv-miso fix.pdf, steps 2-3).
	std::pair<Eigen::MatrixXd, Eigen::MatrixXd> fatten(const Eigen::MatrixXd &x, const int n);

	/// @brief Largest fraction of the step u1 -> u2 over which no element
	/// inverts. The displacements are widened as in hocgv-miso fix.pdf (u1 by
	/// one representable value, u2 by two), so the certificate also covers the
	/// correctly rounded configuration the solver takes after capping the
	/// step, not just the exact segment.
	/// @param rest Rest positions from rest_nodes(); computed here if null.
	/// @param degenerate As in is_valid().
	std::tuple<double, int, double, Tree> max_time_step(
		const int dim,
		const std::vector<basis::ElementBases> &bases,
		const std::vector<basis::ElementBases> &gbases,
		const Eigen::VectorXd &u1,
		const Eigen::VectorXd &u2,
		double precision = .25,
		double threshold = 0,
		const unsigned max_iter = 1'000'000,
		const Eigen::MatrixXd *rest = nullptr,
		const DegenerateMesh *degenerate = nullptr);

} // namespace polyfem::utils

template <>
struct fmt::formatter<polyfem::utils::Tree> : fmt::formatter<fmt::string_view>
{
	format_context::iterator format(const polyfem::utils::Tree &tree, fmt::format_context &ctx) const
	{
		std::ostringstream oss;
		oss << tree;
		return fmt::formatter<fmt::string_view>::format(oss.str(), ctx);
	}
};
