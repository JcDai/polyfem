#ifdef POLYFEM_WITH_MISO

////////////////////////////////////////////////////////////////////////////////
#include <polyfem/utils/Jacobian.hpp>
#include <polyfem/autogen/auto_p_bases.hpp>

#include <iomanip>
#include <iostream>
#include <cmath>

#include <Eigen/Dense>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <polyfem/State.hpp>
#include <polyfem/varforms/VarForm.hpp>

#include "VarFormTestAccess.hpp"
////////////////////////////////////////////////////////////////////////////////

using namespace polyfem;
using namespace polyfem::utils;
using namespace polyfem::autogen;
namespace
{
	std::shared_ptr<State> get_state(int dim, const std::string &material_type = "NeoHookean")
	{
		const std::string path = POLYFEM_DATA_DIR;

		json material;
		material = R"(
        {
            "type": "NeoHookean",
            "E": 20000,
            "nu": 0.3,
            "rho": 1000,
            "phi": 1,
            "psi": 1
        }
        )"_json;

		json in_args = R"(
		{
			"time": {
				"dt": 0.001,
				"tend": 1.0
			},

			"output": {
				"log": {
					"level": "error"
				}
			}

		})"_json;
		in_args["materials"] = material;
		if (dim == 2)
		{
			in_args["geometry"] = R"([{
				"enabled": true,
				"surface_selection": 7
			}])"_json;
			in_args["geometry"][0]["mesh"] = path + "/contact/meshes/2D/simple/circle/circle36.obj";
			in_args["boundary_conditions"] = R"({
				"dirichlet_boundary": [{
					"id": "all",
					"value": [0, 0]
				}],
				"rhs": [10, 10]
			})"_json;
		}
		else
		{
			in_args["geometry"] = R"([{
				"transformation": {
					"scale": [0.1, 1, 1]
				},
				"surface_selection": [
					{
						"id": 1,
						"axis": "z",
						"position": 0.8,
						"relative": true
					},
					{
						"id": 2,
						"axis": "-z",
						"position": 0.2,
						"relative": true
					},
					{
						"id": 3,
						"box": [[0, 0, 0.2], [1, 1, 0.8]],
						"relative": true
					}
				],
				"n_refs": 1
			}])"_json;
			in_args["geometry"][0]["mesh"] = path + "/contact/meshes/3D/simple/bar/bar-6.msh";
			in_args["boundary_conditions"] = R"({
				"neumann_boundary": [{
					"id": 1,
					"value": [1000, 1000, 1000]
				}],
				"pressure_boundary": [{
					"id": 1,
					"value": -2000
				},
				{
					"id": 2,
					"value": -2000
				},
				{
					"id": 3,
					"value": -2000
				}],
				"rhs": [0, 0, 0]
			})"_json;
		}

		auto state = std::make_shared<State>();
		state->init(in_args, true);
		state->set_max_threads(1);

		state->load_mesh();

		test::VarFormTestAccess::prepare(*state->variational_formulation);

		return state;
	}

	template <int N>
	constexpr Eigen::Matrix<double, ((N + 1) * (N + 2)) / 2, 3> upsample_triangle()
	{
		constexpr int num = ((N + 1) * (N + 2)) / 2;

		Eigen::Matrix<double, num, 3> out;
		for (int i = 0, k = 0; i <= N; i++)
			for (int j = 0; i + j <= N; j++, k++)
			{
				std::array<int, 3> arr = {{i, j, N - i - j}};
				std::sort(arr.begin(), arr.end());

				out.row(k) << i, j, N - i - j;
			}

		out.template leftCols<3>() /= N;
		return out;
	}

	template <int N>
	constexpr Eigen::Matrix<double, ((N + 1) * (N + 2) * (N + 3)) / 6, 4> upsample_tetrahedron()
	{
		constexpr int num = ((N + 1) * (N + 2) * (N + 3)) / 6;

		Eigen::Matrix<double, num, 4> out;
		for (int i = 0, k = 0; i <= N; i++)
			for (int j = 0; j <= N; j++)
				for (int l = 0; i + j + l <= N; l++, k++)
				{
					std::array<int, 4> arr = {{i, j, l, N - i - j - l}};
					std::sort(arr.begin(), arr.end());

					out.row(k) << i, j, l, N - i - j - l;
				}

		out.template leftCols<4>() /= N;
		return out;
	}
} // namespace

TEST_CASE("jacobian-evaluate", "[jacobian]")
{
	const double tol = 1e-8;
	constexpr int N = 7;
	for (int dim = 2; dim <= 3; dim++)
	{
		for (int order = 1; order < 4; order++)
		{
			Eigen::MatrixXd cp;
			if (dim == 2)
				autogen::p_nodes_2d(order, cp);
			else
				autogen::p_nodes_3d(order, cp);
			cp += Eigen::MatrixXd::Random(cp.rows(), cp.cols()) * 0.2;

			Eigen::MatrixXd uv;
			if (dim == 2)
				uv = upsample_triangle<N>().leftCols<2>();
			else
				uv = upsample_tetrahedron<N>().leftCols<3>();

			Eigen::VectorXd jac1 = robust_evaluate_jacobian(order, cp, uv);

			std::vector<Eigen::MatrixXd> grads(cp.rows(), Eigen::MatrixXd::Zero(uv.rows(), dim));
			for (int bid = 0; bid < cp.rows(); bid++)
				if (dim == 2)
					p_grad_basis_value_2d(false, order, bid, uv, grads[bid]);
				else
					p_grad_basis_value_3d(false, order, bid, uv, grads[bid]);

			Eigen::VectorXd jac2 = jac1;
			for (int k = 0; k < uv.rows(); k++)
			{
				Eigen::MatrixXd jac_mat;
				jac_mat.setZero(dim, dim);
				for (int bid = 0; bid < cp.rows(); bid++)
					jac_mat += cp.row(bid).transpose() * grads[bid].row(k);

				jac2(k) = jac_mat.determinant();
			}

			Eigen::VectorXd denominator = jac1.array().abs().cwiseMax(jac2.array().abs()).cwiseMax(tol);
			REQUIRE(((jac2 - jac1).array() / denominator.array()).abs().maxCoeff() / tol < 1);
		}
	}
}

TEST_CASE("jacobian-tree", "[jacobian]")
{
	// Leaf node
	Tree t;
	REQUIRE(!t.has_children());
	REQUIRE(t.depth() == 0);
	REQUIRE(t.n_leaves() == 1);

	// One level of children
	t.add_children(4);
	REQUIRE(t.has_children());
	REQUIRE(t.n_children() == 4);
	REQUIRE(t.depth() == 1);
	REQUIRE(t.n_leaves() == 4);

	// merge a two-level source: child(0) has 4 grandchildren
	Tree src;
	src.add_children(4);
	src.child(0).add_children(4);

	const bool changed = t.merge(src, 3);
	REQUIRE(changed);
	// child(0) expanded to depth 2; children 1-3 remain leaves
	REQUIRE(t.depth() == 2);
	REQUIRE(t.n_leaves() == 7); // 4 from child(0) + 1 each from 1,2,3

	// Second merge: no new structure → no change
	const bool changed2 = t.merge(src, 3);
	REQUIRE(!changed2);
}

// Helper: invert one element by dragging a single node far in +x.
// Uses local node 1 of element 1; among the 3 nodes, partition-of-unity
// guarantees at least one has a positive n_x, and node 1 does for this mesh.
static Eigen::VectorXd make_x_flip(const test::VarFormDebugData &debug, const int dim)
{
	Eigen::VectorXd u = Eigen::VectorXd::Zero(debug.n_bases * dim);
	const int dof = (*debug.bases)[1].bases[1].global()[0].index;
	u(dof * dim) = 10.0;
	return u;
}

TEST_CASE("jacobian-is-valid", "[jacobian]")
{
	for (const int dim : {2, 3})
	{
		auto state = get_state(dim);
		const auto debug = test::VarFormTestAccess::debug_data(*state->variational_formulation);
		const int ndof = debug.n_bases * dim;

		// Zero displacement → all elements valid
		{
			const Eigen::VectorXd u = Eigen::VectorXd::Zero(ndof);
			auto [valid, inv_id, tree] = is_valid(dim, *debug.bases, *debug.geometry_bases, u);
			REQUIRE(valid);
			REQUIRE(inv_id == -1);
		}

		// x-flip → det F = -1 everywhere → all elements invalid
		{
			const Eigen::VectorXd u_inv = make_x_flip(debug, dim);
			auto [valid, inv_id, tree] = is_valid(dim, *debug.bases, *debug.geometry_bases, u_inv);
			REQUIRE(!valid);
			REQUIRE(inv_id >= 0);
		}
	}
}

TEST_CASE("jacobian-count-invalid", "[jacobian]")
{
	for (const int dim : {2, 3})
	{
		auto state = get_state(dim);
		const auto debug = test::VarFormTestAccess::debug_data(*state->variational_formulation);
		const int ndof = debug.n_bases * dim;

		// Zero displacement → no invalid elements
		const Eigen::VectorXd u_valid = Eigen::VectorXd::Zero(ndof);
		REQUIRE(count_invalid(dim, *debug.bases, *debug.geometry_bases, u_valid).empty());

		// x-flip → all elements invalid
		const Eigen::VectorXd u_inv = make_x_flip(debug, dim);
		REQUIRE(!count_invalid(dim, *debug.bases, *debug.geometry_bases, u_inv).empty());
	}
}

TEST_CASE("jacobian-max-time-step", "[jacobian]")
{
	for (const int dim : {2, 3})
	{
		auto state = get_state(dim);
		const auto debug = test::VarFormTestAccess::debug_data(*state->variational_formulation);
		const int ndof = debug.n_bases * dim;
		const Eigen::VectorXd u_zero = Eigen::VectorXd::Zero(ndof);
		const Eigen::VectorXd u_inv = make_x_flip(debug, dim);

		// Same start and end (valid → valid) → step = 1
		{
			auto [step, id, inv_s, tree] = max_time_step(dim, *debug.bases, *debug.geometry_bases, u_zero, u_zero);
			REQUIRE(step == Catch::Approx(1.0));
		}

		// Valid → inverted → step ∈ (0, 1)
		auto [step, id, inv_s, tree] = max_time_step(dim, *debug.bases, *debug.geometry_bases, u_zero, u_inv);
		REQUIRE(step > 0.0);
		REQUIRE(step < 1.0);

		// Certified endpoint must be valid: u0 + step*(u_inv - u0) = step*u_inv
		const Eigen::VectorXd u_cert = step * u_inv;
		auto [cert_valid, cert_id, cert_tree] = is_valid(dim, *debug.bases, *debug.geometry_bases, u_cert);
		REQUIRE(cert_valid);
	}
}

#endif

#include <polyfem/utils/Jacobian.hpp>
#include <polyfem/autogen/auto_p_bases.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

namespace
{
	polyfem::basis::ElementBases make_linear_simplex_basis(
		const int dim,
		const int global_offset = 0,
		const Eigen::RowVectorXd &translation = Eigen::RowVectorXd())
	{
		Eigen::MatrixXd nodes;
		if (dim == 2)
			polyfem::autogen::p_nodes_2d(1, nodes);
		else
			polyfem::autogen::p_nodes_3d(1, nodes);

		if (translation.size() == dim)
			nodes.rowwise() += translation;

		polyfem::basis::ElementBases element;
		element.bases.resize(nodes.rows());
		for (int i = 0; i < nodes.rows(); ++i)
		{
			polyfem::RowVectorNd node = nodes.row(i);
			element.bases[i].init(1, global_offset + i, i, node);
			element.bases[i].set_basis([dim, i](const Eigen::MatrixXd &uv, Eigen::MatrixXd &val) {
				if (dim == 2)
					polyfem::autogen::p_basis_value_2d(false, 1, i, uv, val);
				else
					polyfem::autogen::p_basis_value_3d(false, 1, i, uv, val);
			});
			element.bases[i].set_grad([dim, i](const Eigen::MatrixXd &uv, Eigen::MatrixXd &val) {
				if (dim == 2)
					polyfem::autogen::p_grad_basis_value_2d(false, 1, i, uv, val);
				else
					polyfem::autogen::p_grad_basis_value_3d(false, 1, i, uv, val);
			});
		}

		return element;
	}

	Eigen::MatrixXd nodal_displacements_to_matrix(const Eigen::VectorXd &u, const int dim)
	{
		Eigen::MatrixXd U(u.size() / dim, dim);
		for (int i = 0; i < U.rows(); ++i)
			U.row(i) = u.segment(i * dim, dim).transpose();
		return U;
	}
} // namespace

TEST_CASE("jacobian extract nodes on linear simplexes", "[jacobian][utils]")
{
	for (const int dim : {2, 3})
	{
		const auto basis = make_linear_simplex_basis(dim);
		const int n_nodes = dim + 1;

		Eigen::VectorXd u(n_nodes * dim);
		for (int i = 0; i < u.size(); ++i)
			u(i) = 0.05 * (i + 1);

		Eigen::MatrixXd reference_nodes;
		if (dim == 2)
			polyfem::autogen::p_nodes_2d(1, reference_nodes);
		else
			polyfem::autogen::p_nodes_3d(1, reference_nodes);

		const Eigen::MatrixXd cp = polyfem::utils::extract_nodes(dim, basis, basis, u, 1);
		const Eigen::MatrixXd expected = reference_nodes + nodal_displacements_to_matrix(u, dim);

		REQUIRE(cp.rows() == expected.rows());
		REQUIRE(cp.cols() == expected.cols());
		for (int r = 0; r < cp.rows(); ++r)
			for (int c = 0; c < cp.cols(); ++c)
				REQUIRE(cp(r, c) == Catch::Approx(expected(r, c)).margin(1e-12));
	}
}

TEST_CASE("jacobian extract nodes over element ranges", "[jacobian][utils]")
{
	const int dim = 2;
	const int n_nodes_per_element = dim + 1;
	const auto basis0 = make_linear_simplex_basis(dim, 0);
	const auto basis1 = make_linear_simplex_basis(dim, n_nodes_per_element, (Eigen::RowVector2d() << 2.0, -1.0).finished());

	const std::vector<polyfem::basis::ElementBases> bases = {basis0, basis1};
	const std::vector<polyfem::basis::ElementBases> gbases = bases;

	Eigen::VectorXd u(bases.size() * n_nodes_per_element * dim);
	for (int i = 0; i < u.size(); ++i)
		u(i) = 0.01 * (i + 1);

	const Eigen::MatrixXd all_cp = polyfem::utils::extract_nodes(dim, bases, gbases, u, 1);
	const Eigen::MatrixXd first_cp = polyfem::utils::extract_nodes(dim, bases, gbases, u, 1, 1);

	REQUIRE(all_cp.rows() == 2 * n_nodes_per_element);
	REQUIRE(first_cp.rows() == n_nodes_per_element);
	REQUIRE(first_cp.cols() == dim);

	for (int r = 0; r < first_cp.rows(); ++r)
		for (int c = 0; c < first_cp.cols(); ++c)
			REQUIRE(first_cp(r, c) == Catch::Approx(all_cp(r, c)).margin(1e-12));
}

#ifdef POLYFEM_WITH_MISO
#include <fstream>
#include <filesystem>

namespace {
basis::ElementBases cone_test_basis(const Eigen::MatrixXd &cp) {
    basis::ElementBases result;
    result.bases.resize(20);
    for (int i=0;i<20;++i) {
        result.bases[i].init(3,i,i,cp.row(i));
        result.bases[i].set_basis([i](const Eigen::MatrixXd &uv,Eigen::MatrixXd &v) {
            autogen::p_basis_value_3d(true,3,i,uv,v);
        });
        result.bases[i].set_grad([i](const Eigen::MatrixXd &uv,Eigen::MatrixXd &v) {
            autogen::p_grad_basis_value_3d(true,3,i,uv,v);
        });
    }
    return result;
}
}

TEST_CASE("cone metadata remaps input IDs and validates directed edges", "[jacobian][cone]") {
    Eigen::MatrixXd cp;
    autogen::p_nodes_3d(3,cp);
    std::vector<basis::ElementBases> bs{cone_test_basis(cp)};
    Eigen::VectorXi mapping(20);
    for(int i=0;i<20;++i) mapping[i]=19-i;
    const auto path=std::filesystem::temp_directory_path()/"polyfem_cone_edges_test.txt";
    struct Cleanup { std::filesystem::path p; ~Cleanup(){std::filesystem::remove(p);} } cleanup{path};
    { std::ofstream out(path); out << "19 15\n19 10\n19 15\n"; }
    auto metadata=utils::read_degenerate_edges(path.string(),mapping,bs);
    REQUIRE(metadata.size()==1);
    CHECK(metadata[0].vertex==0);
    CHECK(metadata[0].near_nodes==std::vector<int>{4,9});
    for(const std::string bad:{"-1 3\n","0 40\n","19 18\n","19 15 extra\n","15 19\n"}) {
        {std::ofstream out(path);out<<bad;}
        CHECK_THROWS(utils::read_degenerate_edges(path.string(),mapping,bs));
    }
}

TEST_CASE("cone masks use the local corner numbering", "[jacobian][cone]") {
    constexpr int near[4][4]={{-1,4,9,10},{5,-1,6,12},{8,7,-1,14},{11,13,15,-1}};
    for(int vertex=0;vertex<4;++vertex) for(int order=1;order<=2;++order) {
        Eigen::MatrixXd cp;
        autogen::p_nodes_3d(3,cp);
        utils::DegenerateTets metadata(1);
        metadata[0].vertex=vertex;
        for(int w=0;w<4 && metadata[0].near_nodes.size()<size_t(order);++w) {
            if(w==vertex) continue;
            int node=near[vertex][w];
            cp.row(node)=cp.row(vertex);
            metadata[0].near_nodes.push_back(node);
        }
        std::vector<basis::ElementBases> bs{cone_test_basis(cp)};
        CHECK(std::get<0>(utils::is_valid(3,bs,bs,Eigen::VectorXd::Zero(60),0,1000,metadata)));
    }
}

TEST_CASE("conservative checks allow marked cone zeros but detect inversion", "[jacobian][cone]") {
    Eigen::MatrixXd cp;
    autogen::p_nodes_3d(3,cp);
    cp.row(4)=cp.row(0);
    cp.row(9)=cp.row(0);
    std::vector<basis::ElementBases> bs{cone_test_basis(cp)};
    const Eigen::VectorXd zero=Eigen::VectorXd::Zero(60);
    utils::DegenerateTets metadata(1);
    metadata[0].vertex=0;
    metadata[0].near_nodes={4,9};
    CHECK_FALSE(std::get<0>(utils::is_valid(3,bs,bs,zero,0,1000)));
    CHECK(std::get<0>(utils::is_valid(3,bs,bs,zero,0,1000,metadata)));
    CHECK(utils::count_invalid(3,bs,bs,zero,1000,metadata).empty());
    Eigen::VectorXd translation=Eigen::VectorXd::Constant(60,0.125);
    CHECK(std::get<0>(utils::max_time_step(3,bs,bs,zero,translation,1e-5,0,1000,metadata))>=1.0);
    // Reflection keeps the declared corner zeros but reverses orientation.
    Eigen::VectorXd inverted=zero;
    for(int i=0;i<20;++i) inverted[3*i]=-2*cp(i,0);
    CHECK_FALSE(std::get<0>(utils::is_valid(3,bs,bs,inverted,0,1000,metadata)));
    CHECK(std::get<0>(utils::max_time_step(3,bs,bs,zero,inverted,1e-5,0,1000,metadata))<0.51);
    // A first collapse at t=1 marks only the terminal space-time vertex.
    Eigen::MatrixXd regular;
    autogen::p_nodes_3d(3,regular);
    std::vector<basis::ElementBases> regular_bs{cone_test_basis(regular)};
    Eigen::VectorXd collapse=zero;
    for(int i : {4,9}) collapse.segment<3>(3*i)=-regular.row(i).transpose();
    CHECK(std::get<0>(utils::max_time_step(3,regular_bs,regular_bs,zero,collapse,1e-5,0,1000,metadata))>=1.0);
    // A metadata file cannot turn a separated control point into a zero.
    Eigen::VectorXd separated=zero;
    separated[4*3]=-0.25;
    CHECK_FALSE(std::get<0>(utils::is_valid(3,bs,bs,separated,0,1000,metadata)));
}
#endif

#ifdef POLYFEM_WITH_MISO
TEST_CASE("conservative conversion preserves curved Lagrange geometry", "[jacobian][cone]") {
    Eigen::MatrixXd uv;
    autogen::p_nodes_3d(3,uv);
    auto cp=uv;
    cp.col(0).array() += 0.25*uv.col(0).array()*uv.col(1).array();
    auto element=cone_test_basis(cp);
    for(int i=0;i<20;++i) {
        element.bases[i].set_basis([i](const Eigen::MatrixXd &x,Eigen::MatrixXd &v) {
            autogen::p_basis_value_3d(false,3,i,x,v);
        });
        element.bases[i].set_grad([i](const Eigen::MatrixXd &x,Eigen::MatrixXd &v) {
            autogen::p_grad_basis_value_3d(false,3,i,x,v);
        });
    }
    std::vector<basis::ElementBases> bs{element};
    CHECK(std::get<0>(utils::is_valid(3,bs,bs,Eigen::VectorXd::Zero(60),0,1000)));
    Eigen::VectorXd flip=Eigen::VectorXd::Zero(60);
    for(int i=0;i<20;++i) flip[3*i]=-2*cp(i,0);
    CHECK_FALSE(std::get<0>(utils::is_valid(3,bs,bs,flip,0,1000)));
    // Linear geometry and cubic displacement require Bernstein degree elevation.
    std::vector<basis::ElementBases> linear{make_linear_simplex_basis(3)};
    std::vector<basis::ElementBases> cubic{cone_test_basis(uv)};
    CHECK(std::get<0>(utils::is_valid(3,cubic,linear,Eigen::VectorXd::Constant(60,0.125),0,1000)));
}
#endif

#ifdef POLYFEM_WITH_MISO
TEST_CASE("extract_nodes samples Bernstein geometry rather than returning controls", "[jacobian][cone]") {
    Eigen::MatrixXd controls;
    autogen::p_nodes_3d(3, controls);
    controls.row(4) = controls.row(0);
    const auto element = cone_test_basis(controls);
    const Eigen::VectorXd zero = Eigen::VectorXd::Zero(60);
    const auto sampled = utils::extract_nodes(3, element, element, zero, 3);
    // At t=1/3: B0*0 + B1*0 + B2*(2/3) + B3*1 = 5/27.
    CHECK(controls(4, 0) == 0);
    CHECK(sampled(4, 0) == Catch::Approx(5.0 / 27.0));
    std::vector<basis::ElementBases> elements{element};
    const auto batch = utils::extract_nodes(3, elements, elements, zero, 3);
    CHECK(batch.isApprox(sampled));
}
#endif

#ifdef POLYFEM_WITH_MISO
#include <polyfem/solver/forms/ElasticForm.hpp>
#include <polyfem/assembler/AMIPSEnergy.hpp>

TEST_CASE("ElasticForm integrates marked cones with interior quadrature", "[jacobian][cone][cone_quadrature]") {
    Eigen::MatrixXd cp;
    autogen::p_nodes_3d(3, cp);
    std::vector<basis::ElementBases> bs{cone_test_basis(cp)};
    const auto geometry = bs;
    assembler::AMIPSEnergy amips;
    amips.set_size(3);
    amips.add_multimaterial(0, json{{"use_rest_pose", true}}, Units(), "");
    assembler::AssemblyValsCache cache;
    cache.init_empty(false);
    solver::ElasticForm form(20, bs, geometry, amips, cache, 0, 1, true, 0,
                         solver::ElementInversionCheck::Conservative, 1000);
    cache.init(true, bs, geometry);
    Eigen::VectorXd u = Eigen::VectorXd::Zero(60);
    u.segment<3>(12) = -cp.row(4).transpose();
    u.segment<3>(27) = -cp.row(9).transpose();
    CHECK_FALSE(std::isfinite(form.value(u)));
    const auto path = std::filesystem::temp_directory_path() / "polyfem_cone_quadrature_test.txt";
    struct Cleanup { std::filesystem::path p; ~Cleanup(){std::filesystem::remove(p);} } cleanup{path};
    { std::ofstream out(path); out << "0 4\n0 9\n"; }
    Eigen::VectorXi mapping = Eigen::VectorXi::LinSpaced(20, 0, 19);
    form.load_degenerate_edges(path.string(), mapping);
    auto check = [&]() {
        quadrature::Quadrature q;
        bs[0].compute_quadrature(q);
        CHECK(q.points.minCoeff() > 0);
        CHECK(q.points.rowwise().sum().maxCoeff() < 1);
        CHECK(q.weights.minCoeff() > 0);
        CHECK(q.weights.sum() == Catch::Approx(1.0 / 6.0));
        CHECK(std::isfinite(form.value(u)));
        Eigen::VectorXd gradient;
        form.first_derivative(u, gradient);
        CHECK(gradient.allFinite());
        CHECK(form.is_step_collision_free(u, u));
    };
    check();
    quadrature::Quadrature before, after;
    bs[0].compute_quadrature(before);
    Eigen::VectorXd trial = u;
    trial[16 * 3 + 2] += 3;
    CHECK(form.max_step_size(u, trial) < 1);
    const json info = json::object();
    const Eigen::VectorXd zero = Eigen::VectorXd::Zero(60);
    form.post_step(polysolve::nonlinear::PostStepData(0, info, u, zero));
    bs[0].compute_quadrature(after);
    CHECK(after.size() > before.size());
    check();
    form.load_degenerate_edges("", mapping);
    CHECK_FALSE(std::isfinite(form.value(u)));
}
#endif
