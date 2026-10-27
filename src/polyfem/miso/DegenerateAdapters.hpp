#pragma once
// Eigen glue for the P3 degenerate checks (generate.py: DegenerateChecks).
//
// Unlike the classes in EigenAdapters.hpp, these take the map's Bernstein
// coefficients p = rest + displacement, already formed in interval
// arithmetic by the caller, as one input per coordinate. Control points come
// in polyfem's node order and are permuted into miso's lattice order with the
// same tables as the Lagrange nodes (tri3_perm, tet3_perm): node i of
// polyfem's P3 element and Bernstein coefficient i refer to the same lattice
// point.
#include "EigenAdapters.hpp"

#include "generated/DegenerateRules.hpp"
#include "generated/P3TriDegVal.hpp"
#include "generated/P3TriDegCGV.hpp"
#include "generated/P3TetDegVal.hpp"
#include "generated/P3TetDegCGV.hpp"
#include "generated/JacEval_P3TriBernstein.hpp"
#include "generated/JacEval_P3TetBernstein.hpp"

#include <vector>

namespace miso
{
	/// Bernstein coefficients of every element, one row per control point and
	/// one column per coordinate, as intervals.
	struct IntervalCoefficients
	{
		int dim = 0;
		std::vector<Real> data; // row-major: data[row * dim + col]

		IntervalCoefficients() = default;
		IntervalCoefficients(const int rows, const int dim_) : dim(dim_), data(size_t(rows) * dim_, Real(0.)) {}
		Real &operator()(const int r, const int c) { return data[size_t(r) * dim + c]; }
		const Real &operator()(const int r, const int c) const { return data[size_t(r) * dim + c]; }
	};

	template <int N>
	inline RealVector<N> rv(const IntervalCoefficients &m, int offset, int col, const int *perm)
	{
		RealVector<N> v;
		for (int i = 0; i < N; ++i)
			v[i] = m(offset + perm[i], col);
		return v;
	}

	inline P3TriDegVal make_p3tri_degval(const IntervalCoefficients &p, int o)
	{
		return {rv<10>(p, o, 0, tri3_perm), rv<10>(p, o, 1, tri3_perm)};
	}

	inline P3TetDegVal make_p3tet_degval(const IntervalCoefficients &p, int o)
	{
		return {rv<20>(p, o, 0, tet3_perm), rv<20>(p, o, 1, tet3_perm), rv<20>(p, o, 2, tet3_perm)};
	}

	inline P3TriDegCGV make_p3tri_degcgv(const IntervalCoefficients &p0, const IntervalCoefficients &p1, int o)
	{
		return {rv<10>(p0, o, 0, tri3_perm), rv<10>(p0, o, 1, tri3_perm), rv<10>(p1, o, 0, tri3_perm), rv<10>(p1, o, 1, tri3_perm)};
	}

	inline P3TetDegCGV make_p3tet_degcgv(const IntervalCoefficients &p0, const IntervalCoefficients &p1, int o)
	{
		return {rv<20>(p0, o, 0, tet3_perm), rv<20>(p0, o, 1, tet3_perm), rv<20>(p0, o, 2, tet3_perm),
				rv<20>(p1, o, 0, tet3_perm), rv<20>(p1, o, 1, tet3_perm), rv<20>(p1, o, 2, tet3_perm)};
	}

	/// det J of one element at reference point (row k of uv), with the
	/// expression the degenerate checks are generated from.
	inline Real eval_p3tri_jac_bernstein(const IntervalCoefficients &p, int o, const Eigen::MatrixXd &uv, int k)
	{
		return JacEval_P3TriBernstein(rv<10>(p, o, 0, tri3_perm), rv<10>(p, o, 1, tri3_perm), Real(uv(k, 0)), Real(uv(k, 1)));
	}

	inline Real eval_p3tet_jac_bernstein(const IntervalCoefficients &p, int o, const Eigen::MatrixXd &uv, int k)
	{
		return JacEval_P3TetBernstein(rv<20>(p, o, 0, tet3_perm), rv<20>(p, o, 1, tet3_perm), rv<20>(p, o, 2, tet3_perm),
									  Real(uv(k, 0)), Real(uv(k, 1)), Real(uv(k, 2)));
	}

	/// polyfem vertex -> index of the same point in miso's domain vertices
	/// ((0,0), (0,1), (1,0) and (0,0,0), (0,0,1), (0,1,0), (1,0,0)).
	constexpr unsigned tri_domain_vertex[3] = {0, 2, 1};
	constexpr unsigned tet_domain_vertex[4] = {0, 3, 2, 1};
} // namespace miso
