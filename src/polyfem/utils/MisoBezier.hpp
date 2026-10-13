#pragma once

// MISO's degenerate-check generators use Bernstein geometry. Keep the public
// extract_nodes API (point samples) unchanged; assemble coefficient intervals
// directly from the FE bases for conservative checks.
#include "Jacobian.hpp"
#include "EigenAdapters.hpp"
#include <polyfem/autogen/auto_p_bases.hpp>
#include <array>
#include <map>

namespace polyfem::utils::miso_detail
{
	using miso::Real;
	struct IntervalScope
	{
		IntervalScope() { miso::RealInterval::init(); }
		~IntervalScope() { miso::RealInterval::deinit(); }
	};
	using Matrix = std::vector<std::vector<Real>>;
	struct BezierData
	{
		Matrix rows;
	};

	inline int factorial(int n)
	{
		int v = 1;
		for (int i = 2; i <= n; ++i)
			v *= i;
		return v;
	}
	inline int multinomial(const std::vector<int> &a)
	{
		int n = 0, den = 1;
		for (int v : a)
		{
			if (v < 0)
				return 0;
			n += v;
			den *= factorial(v);
		}
		return factorial(n) / den;
	}
	inline Eigen::MatrixXd nodes(int dim, int p)
	{
		Eigen::MatrixXd x;
		if (dim == 3)
			autogen::p_nodes_3d(p, x);
		else
			autogen::p_nodes_2d(p, x);
		return x;
	}
	inline std::vector<std::vector<int>> indices(int dim, int p)
	{
		auto x = nodes(dim, p);
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
	// Build the exact-rational Lagrange-to-Bernstein transform with outward-rounded
	// interval arithmetic. No floating-point inverse is treated as exact.
	inline Matrix lagrange_to_bernstein(int dim, int p)
	{
		const auto a = indices(dim, p);
		const int n = a.size();
		Matrix b(n, std::vector<Real>(2 * n, Real(0)));
		for (int i = 0; i < n; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				Real v(multinomial(a[j]));
				for (int d = 0; d <= dim; ++d)
					for (int k = 0; k < a[j][d]; ++k)
						v = v * Real(a[i][d]) / Real(p);
				b[i][j] = v;
			}
			b[i][n + i] = Real(1);
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
				throw std::runtime_error("Singular Bernstein conversion");
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
		Matrix result(n, std::vector<Real>(n));
		for (int i = 0; i < n; ++i)
			for (int j = 0; j < n; ++j)
				result[i][j] = b[i][n + j];
		return result;
	}
	inline bool is_bernstein(const basis::ElementBases &basis, int dim, int p)
	{
		auto x = nodes(dim, p);
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
			throw std::runtime_error("Unsupported basis in conservative Jacobian check");
		return bernstein;
	}
	inline BezierData extract_bezier(int dim, const std::vector<basis::ElementBases> &bases,
									 const std::vector<basis::ElementBases> &gbases, const Eigen::VectorXd &u, int p)
	{
		const auto target = indices(dim, p);
		const int n = target.size();
		BezierData result{Matrix(bases.size() * n, std::vector<Real>(dim, Real(0)))};
		std::map<int, Matrix> conversions;
		for (size_t e = 0; e < bases.size(); ++e)
		{
			for (int geometry = 0; geometry < 2; ++geometry)
			{
				const auto &bs = geometry ? gbases[e] : bases[e];
				const int q = bs.bases.front().order();
				const auto source = indices(dim, q);
				if (source.size() != bs.bases.size())
					throw std::runtime_error("Conservative check requires simplex bases");
				Matrix c(source.size(), std::vector<Real>(dim, Real(0)));
				for (size_t i = 0; i < source.size(); ++i)
					for (const auto &g : bs.bases[i].global())
						for (int d = 0; d < dim; ++d)
							c[i][d] = c[i][d] + Real(g.val) * Real(geometry ? g.node[d] : u[g.index * dim + d]);
				if (!is_bernstein(bs, dim, q))
				{
					if (!conversions.count(q))
						conversions.emplace(q, lagrange_to_bernstein(dim, q));
					Matrix converted(c.size(), std::vector<Real>(dim, Real(0)));
					for (size_t i = 0; i < c.size(); ++i)
						for (size_t j = 0; j < c.size(); ++j)
							for (int d = 0; d < dim; ++d)
								converted[i][d] = converted[i][d] + conversions.at(q)[i][j] * c[j][d];
					c = std::move(converted);
				}
				// Exact Bernstein degree elevation, including linear geometry with
				// cubic displacement. Equal degrees use coefficient copies.
				for (int i = 0; i < n; ++i)
					for (size_t j = 0; j < source.size(); ++j)
					{
						std::vector<int> delta(dim + 1);
						for (int d = 0; d <= dim; ++d)
							delta[d] = target[i][d] - source[j][d];
						const int factor = multinomial(delta);
						if (!factor)
							continue;
						const Real weight = Real(multinomial(source[j]) * factor) / Real(multinomial(target[i]));
						for (int d = 0; d < dim; ++d)
							result.rows[e * n + i][d] = result.rows[e * n + i][d] + weight * c[j][d];
					}
			}
		}
		return result;
	}

	template <int N>
	miso::RealVector<N> rv(const BezierData &cp, int o, int d, const int *perm)
	{
		miso::RealVector<N> v;
		for (int i = 0; i < N; ++i)
			v[i] = cp.rows.at(o + perm[i]).at(d);
		return v;
	}
#define TRI_VAL(DEGREE, N) \
	inline miso::P##DEGREE##TriVal make_p##DEGREE##tri_val(const BezierData &a, int o) { return {rv<N>(a, o, 0, miso::tri##DEGREE##_perm), rv<N>(a, o, 1, miso::tri##DEGREE##_perm)}; }
#define TET_VAL(DEGREE, N) \
	inline miso::P##DEGREE##TetVal make_p##DEGREE##tet_val(const BezierData &a, int o) { return {rv<N>(a, o, 0, miso::tet##DEGREE##_perm), rv<N>(a, o, 1, miso::tet##DEGREE##_perm), rv<N>(a, o, 2, miso::tet##DEGREE##_perm)}; }
#define TRI_CGV(DEGREE, N) \
	inline miso::P##DEGREE##TriCGV make_p##DEGREE##tri_cgv(const BezierData &a, const BezierData &b, int o) { return {rv<N>(a, o, 0, miso::tri##DEGREE##_perm), rv<N>(a, o, 1, miso::tri##DEGREE##_perm), rv<N>(b, o, 0, miso::tri##DEGREE##_perm), rv<N>(b, o, 1, miso::tri##DEGREE##_perm)}; }
#define TET_CGV(DEGREE, N) \
	inline miso::P##DEGREE##TetCGV make_p##DEGREE##tet_cgv(const BezierData &a, const BezierData &b, int o) { return {rv<N>(a, o, 0, miso::tet##DEGREE##_perm), rv<N>(a, o, 1, miso::tet##DEGREE##_perm), rv<N>(a, o, 2, miso::tet##DEGREE##_perm), rv<N>(b, o, 0, miso::tet##DEGREE##_perm), rv<N>(b, o, 1, miso::tet##DEGREE##_perm), rv<N>(b, o, 2, miso::tet##DEGREE##_perm)}; }
	TRI_VAL(2, 6)
	TRI_VAL(3, 10) TRI_VAL(4, 15)
		TET_VAL(2, 10) TET_VAL(3, 20)
			TRI_CGV(1, 3) TRI_CGV(2, 6) TRI_CGV(3, 10) TRI_CGV(4, 15)
				TET_CGV(1, 4) TET_CGV(2, 10) TET_CGV(3, 20)
#undef TRI_VAL
#undef TET_VAL
#undef TRI_CGV
#undef TET_CGV

					template <class Problem>
					Problem mark(Problem problem, const BezierData &a, const BezierData *b,
								 int offset, int element, const DegenerateTets &metadata)
	{
		if (metadata.empty())
			return problem;
		const auto &d = metadata.at(element);
		if (d.vertex < 0)
			return problem;
		auto collapsed = [&](const BezierData &cp, int near) {
			for (int axis = 0; axis < 3; ++axis)
			{
				const Real diff = cp.rows[offset + near][axis] - cp.rows[offset + d.vertex][axis];
				if (lower(diff) > 0 || upper(diff) < 0)
					return false;
			}
			return true;
		};
		unsigned at_start = 0, at_end = 0, common = 0;
		for (int near : d.near_nodes)
		{
			const bool start = collapsed(a, near), end = b && collapsed(*b, near);
			at_start += start;
			at_end += end;
			common += start && end;
		}
		constexpr unsigned map[4] = {0, 3, 2, 1};
		const unsigned v = map[d.vertex];
		unsigned mask = 0, order = 0;
		if constexpr (!Problem::hasObjective)
		{
			mask = 1u << v;
			order = at_start;
		}
		else if (common)
		{
			// The same collapsed directions persist throughout the linear blend.
			mask = 3u << (2 * v);
			order = common;
		}
		else if (at_start && !at_end)
		{
			mask = 1u << (2 * v);
			order = at_start;
		}
		else if (at_end && !at_start)
		{
			mask = 1u << (2 * v + 1);
			order = at_end;
		}
		// Do not merge two isolated endpoint zeros into a time-edge declaration.
		// Unsupported combinations retain ordinary checks for the extra zeros.
		if (order)
			problem.degenerateState = Problem::degenerateStateFromVertices(mask, order);
		return problem;
	}
} // namespace polyfem::utils::miso_detail
