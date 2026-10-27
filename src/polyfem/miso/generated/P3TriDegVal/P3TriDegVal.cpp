#include "../P3TriDegVal.hpp"
#include <stdexcept>
namespace miso {
	namespace {
		constexpr std::array<unsigned, 4> kDegenerateStateVertices = {0u, 4u, 2u, 1u};
		constexpr std::array<unsigned, 4> kDegenerateStateOrders = {0u, 1u, 1u, 1u};
		constexpr std::array<std::array<unsigned, 4>, 4> kDegenerateTransitions = {{
			{0, 0, 0, 0},
			{0, 1, 0, 0},
			{0, 0, 2, 0},
			{3, 0, 0, 0},
		}};
	}

	unsigned P3TriDegVal::degenerateStateFromVertices(unsigned vertexMask, unsigned order) {
		if (vertexMask == 0u) return 0u;
		for (unsigned s=0; s<4; ++s) {
			if (kDegenerateStateVertices[s] == vertexMask && kDegenerateStateOrders[s] == order) return s;
		}
		throw std::invalid_argument("No degenerate state for this vertex set and order");
	}
	unsigned P3TriDegVal::degenerateStateVertexMask(unsigned s) { return kDegenerateStateVertices[s]; }
	unsigned P3TriDegVal::degenerateStateOrder(unsigned s) { return kDegenerateStateOrders[s]; }
	unsigned P3TriDegVal::degenerateTransition(unsigned s, unsigned q) { return kDegenerateTransitions[s][q]; }

	P3TriDegVal::P3TriDegVal(const P3TriDegVal &parent, unsigned q,
		const RealVector<15> &m0,
		const RealVector<3> &x0,
		const RealVector<3> &x1
	) :
	m0(m0),
	x0(x0),
	x1(x1)
	{ inherit(parent, q); }

	P3TriDegVal::P3TriDegVal(
		const RealVector<10> &px,
		const RealVector<10> &py
	) :
	m0(LB_2p4(CL_m0(px, py))),
	x0{0, 0, 1},
	x1{0, 1, 0}
	{
		history = std::make_shared<SubdivHistory>();
	}

	void P3TriDegVal::inherit(const P3TriDegVal &parent, unsigned q) {
		depth = parent.depth + 1;
		history = std::make_shared<SubdivHistory>(q, parent.history);
		degenerateState = kDegenerateTransitions[parent.degenerateState][q];
	}
	RealInterval P3TriDegVal::inclusion(unsigned i) const {
		switch (i) {
			case 0:
			switch (degenerateState) {
				case 1: return m0.inclusionDegenerate<14>();
				case 2: return m0.inclusionDegenerate<4>();
				case 3: return m0.inclusionDegenerate<0>();
				default: return m0.inclusion();
			}
			default: throw std::logic_error("Undefined inclusion function");
		}
	}
	static constexpr std::array<std::array<unsigned, 2>, 10> kPairs_m0_0 = {{{5u, 0u}, {6u, 1u}, {7u, 2u}, {8u, 3u}, {9u, 5u}, {10u, 6u}, {11u, 7u}, {12u, 9u}, {13u, 10u}, {14u, 12u}}};
	static constexpr std::array<std::array<unsigned, 2>, 10> kPairs_m0_1 = {{{1u, 0u}, {2u, 1u}, {3u, 2u}, {4u, 3u}, {6u, 5u}, {7u, 6u}, {8u, 7u}, {10u, 9u}, {11u, 10u}, {13u, 12u}}};
	double P3TriDegVal::sampleError_m0() const {
		#ifdef MISO_FP_ARITHMETIC
		return 0.;
		#else
		const Real L00 = x0[2] - x0[0];
		const Real L01 = x0[1] - x0[0];
		const Real L10 = x1[2] - x1[0];
		const Real L11 = x1[1] - x1[0];
		const Real det = L00 * L11 - L01 * L10;
		if (lower(det) <= 0. && upper(det) >= 0.) return infinity;
		const Real adet = abs(det);
		double vmax = 0.;
		vmax = std::max(vmax, upper(abs(x0[0])));
		vmax = std::max(vmax, upper(abs(x1[0])));
		vmax = std::max(vmax, upper(abs(x0[2])));
		vmax = std::max(vmax, upper(abs(x1[2])));
		vmax = std::max(vmax, upper(abs(x0[1])));
		vmax = std::max(vmax, upper(abs(x1[1])));
		const Real eps = Real(0x1.4000000000000p-50) * Real(vmax);
		Real sumdelta(0.);
		Real err(0.);
		const Real delta0 = (abs(L11) + abs(-L01)) / adet * eps;
		sumdelta = sumdelta + delta0;
		double D0 = 0.;
		for (const auto &p : kPairs_m0_0) D0 = std::max(D0, upper(abs(m0[p[0]] - m0[p[1]])));
		err = err + delta0 * Real(D0);
		const Real delta1 = (abs(-L10) + abs(L00)) / adet * eps;
		sumdelta = sumdelta + delta1;
		double D1 = 0.;
		for (const auto &p : kPairs_m0_1) D1 = std::max(D1, upper(abs(m0[p[0]] - m0[p[1]])));
		err = err + delta1 * Real(D1);
		if (!(upper(sumdelta) < 0x1.77189ea1db0c8p-4)) return infinity;
		return upper(Real(4.) * err);
		#endif
	}
	RealVector<P3TriDegVal::numSamples> P3TriDegVal::sampleLanes_m0() const {
		RealVector<numSamples> v = SQ_m0(m0);
		#ifndef MISO_FP_ARITHMETIC
		const double e = sampleError_m0();
		const RealInterval inc = m0.inclusion();
		for (unsigned l = 0; l < numSamples; ++l) {
			if (!(e < infinity)) { v[l] = Real(infinity); continue; }
			const Real w = v[l] + _R(-e, e);
			v[l] = _R(std::max(lower(w), lower(inc)), std::min(upper(w), upper(inc)));
		}
		#endif
		return v;
	}
	RealVector<P3TriDegVal::numSamples> P3TriDegVal::sample(unsigned i) const {
		switch (i) {
			case 0: {
				const RealVector<numSamples> _s_m0 = sampleLanes_m0();
				return _s_m0;
			}
			default: throw std::logic_error("Undefined sampling function");
		}
	}
	RealVector<15> P3TriDegVal::degenerateZeroed_m0() const {
		RealVector<15> _b = m0;
		switch (degenerateState) {
			case 1: _b[14] = Real(0); break;
			case 2: _b[4] = Real(0); break;
			case 3: _b[0] = Real(0); break;
			default: break;
		}
		return _b;
	}
	template<> std::array<P3TriDegVal, P3TriDegVal::schemes[0]> P3TriDegVal::split_impl<0>() const {
		auto _r_m0 = subdiv_0_2p4(degenerateZeroed_m0());
		auto _r_x0 = subdiv_0_2p1(x0);
		auto _r_x1 = subdiv_0_2p1(x1);
		return {
			P3TriDegVal{*this, 0,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0]
			},
			P3TriDegVal{*this, 1,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1]
			},
			P3TriDegVal{*this, 2,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2]
			},
			P3TriDegVal{*this, 3,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3]
			},
		};
	}
	std::ostream &operator<<(std::ostream &out, const P3TriDegVal &s) {
		for (unsigned i=0; i<s.numVertices; ++i) {
			out << s.getVertex(i) << ' ';
		}
		return out;
	}
}
