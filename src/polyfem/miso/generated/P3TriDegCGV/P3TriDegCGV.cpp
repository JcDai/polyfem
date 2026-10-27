#include "../P3TriDegCGV.hpp"
#include <stdexcept>
namespace miso {
	namespace {
		constexpr std::array<unsigned, 4> kDegenerateStateVertices = {0u, 48u, 12u, 3u};
		constexpr std::array<unsigned, 4> kDegenerateStateOrders = {0u, 1u, 1u, 1u};
		constexpr std::array<std::array<unsigned, 14>, 4> kDegenerateTransitions = {{
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0},
			{0, 0, 0, 0, 2, 2, 0, 0, 2, 2, 0, 0, 2, 0},
			{3, 3, 0, 0, 0, 0, 0, 0, 3, 3, 3, 0, 0, 0},
		}};
	}

	unsigned P3TriDegCGV::degenerateStateFromVertices(unsigned vertexMask, unsigned order) {
		if (vertexMask == 0u) return 0u;
		for (unsigned s=0; s<4; ++s) {
			if (kDegenerateStateVertices[s] == vertexMask && kDegenerateStateOrders[s] == order) return s;
		}
		throw std::invalid_argument("No degenerate state for this vertex set and order");
	}
	unsigned P3TriDegCGV::degenerateStateVertexMask(unsigned s) { return kDegenerateStateVertices[s]; }
	unsigned P3TriDegCGV::degenerateStateOrder(unsigned s) { return kDegenerateStateOrders[s]; }
	unsigned P3TriDegCGV::degenerateTransition(unsigned s, unsigned q) { return kDegenerateTransitions[s][q]; }

	namespace {
		constexpr std::array<unsigned, 4> kAlternateScheme = {2u, 2u, 2u, 2u};
		constexpr std::array<unsigned, 14> kAdvancesAlternation = {1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 0u, 0u, 1u, 1u, 1u, 1u};
	}
	unsigned P3TriDegCGV::alternateScheme(unsigned s) { return kAlternateScheme[s]; }
	unsigned P3TriDegCGV::effectiveScheme(unsigned s) const {
		if (s == 0u && (alternation & 1u)) {
			const unsigned a = kAlternateScheme[degenerateState];
			if (a) return a;
		}
		return s;
	}

	P3TriDegCGV::P3TriDegCGV(const P3TriDegCGV &parent, unsigned q,
		const RealVector<45> &m0,
		const RealVector<6> &x0,
		const RealVector<6> &x1,
		const RealVector<6> &x2
	) :
	m0(m0),
	x0(x0),
	x1(x1),
	x2(x2)
	{ inherit(parent, q); }

	P3TriDegCGV::P3TriDegCGV(
		const RealVector<10> &p0x,
		const RealVector<10> &p0y,
		const RealVector<10> &p1x,
		const RealVector<10> &p1y
	) :
	m0(LB_2p4_1p2(CL_m0(p0x, p0y, p1x, p1y))),
	x0{0, 0, 0, 0, 1, 1},
	x1{0, 0, 1, 1, 0, 0},
	x2{0, 1, 0, 1, 0, 1}
	{
		history = std::make_shared<SubdivHistory>();
	}

	void P3TriDegCGV::inherit(const P3TriDegCGV &parent, unsigned q) {
		depth = parent.depth + 1;
		history = std::make_shared<SubdivHistory>(q, parent.history);
		degenerateState = kDegenerateTransitions[parent.degenerateState][q];
		alternation = parent.alternation + kAdvancesAlternation[q];
	}
	RealInterval P3TriDegCGV::inclusion(unsigned i) const {
		switch (i) {
			case 0:
			switch (degenerateState) {
				case 1: return m0.inclusionDegenerate<42,43,44>();
				case 2: return m0.inclusionDegenerate<12,13,14>();
				case 3: return m0.inclusionDegenerate<0,1,2>();
				default: return m0.inclusion();
			}
			case 1: return x2.inclusion();
			default: throw std::logic_error("Undefined inclusion function");
		}
	}
	static constexpr std::array<std::array<unsigned, 2>, 30> kPairs_m0_0 = {{{15u, 0u}, {16u, 1u}, {17u, 2u}, {18u, 3u}, {19u, 4u}, {20u, 5u}, {21u, 6u}, {22u, 7u}, {23u, 8u}, {24u, 9u}, {25u, 10u}, {26u, 11u}, {27u, 15u}, {28u, 16u}, {29u, 17u}, {30u, 18u}, {31u, 19u}, {32u, 20u}, {33u, 21u}, {34u, 22u}, {35u, 23u}, {36u, 27u}, {37u, 28u}, {38u, 29u}, {39u, 30u}, {40u, 31u}, {41u, 32u}, {42u, 36u}, {43u, 37u}, {44u, 38u}}};
	static constexpr std::array<std::array<unsigned, 2>, 30> kPairs_m0_1 = {{{3u, 0u}, {4u, 1u}, {5u, 2u}, {6u, 3u}, {7u, 4u}, {8u, 5u}, {9u, 6u}, {10u, 7u}, {11u, 8u}, {12u, 9u}, {13u, 10u}, {14u, 11u}, {18u, 15u}, {19u, 16u}, {20u, 17u}, {21u, 18u}, {22u, 19u}, {23u, 20u}, {24u, 21u}, {25u, 22u}, {26u, 23u}, {30u, 27u}, {31u, 28u}, {32u, 29u}, {33u, 30u}, {34u, 31u}, {35u, 32u}, {39u, 36u}, {40u, 37u}, {41u, 38u}}};
	double P3TriDegCGV::sampleError_m0() const {
		#ifdef MISO_FP_ARITHMETIC
		return 0.;
		#else
		const Real L00 = x0[4] - x0[0];
		const Real L01 = x0[2] - x0[0];
		const Real L10 = x1[4] - x1[0];
		const Real L11 = x1[2] - x1[0];
		const Real det = L00 * L11 - L01 * L10;
		if (lower(det) <= 0. && upper(det) >= 0.) return infinity;
		const Real adet = abs(det);
		double vmax = 0.;
		vmax = std::max(vmax, upper(abs(x0[0])));
		vmax = std::max(vmax, upper(abs(x1[0])));
		vmax = std::max(vmax, upper(abs(x0[4])));
		vmax = std::max(vmax, upper(abs(x1[4])));
		vmax = std::max(vmax, upper(abs(x0[2])));
		vmax = std::max(vmax, upper(abs(x1[2])));
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
	RealVector<P3TriDegCGV::numSamples> P3TriDegCGV::sampleLanes_m0() const {
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
	RealVector<P3TriDegCGV::numSamples> P3TriDegCGV::sample(unsigned i) const {
		switch (i) {
			case 0: {
				const RealVector<numSamples> _s_m0 = sampleLanes_m0();
				return _s_m0;
			}
			case 1: {
				const RealVector<numSamples> _s_x2 = {x2[0], x2[1], x2[0], x2[1], x2[0], x2[1], x2[0], x2[1], x2[0], x2[1], x2[0], x2[1]};
				return _s_x2;
			}
			default: throw std::logic_error("Undefined sampling function");
		}
	}
	RealVector<45> P3TriDegCGV::degenerateZeroed_m0() const {
		RealVector<45> _b = m0;
		switch (degenerateState) {
			case 1: _b[42] = Real(0); _b[43] = Real(0); _b[44] = Real(0); break;
			case 2: _b[12] = Real(0); _b[13] = Real(0); _b[14] = Real(0); break;
			case 3: _b[0] = Real(0); _b[1] = Real(0); _b[2] = Real(0); break;
			default: break;
		}
		return _b;
	}
	template<> std::array<P3TriDegCGV, P3TriDegCGV::schemes[0]> P3TriDegCGV::split_impl<0>() const {
		auto _r_m0 = subdiv_0_2p4_1p2(degenerateZeroed_m0());
		auto _r_x0 = subdiv_0_2p1_1p1(x0);
		auto _r_x1 = subdiv_0_2p1_1p1(x1);
		auto _r_x2 = subdiv_0_2p1_1p1(x2);
		return {
			P3TriDegCGV{*this, 0,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0]
			},
			P3TriDegCGV{*this, 1,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1]
			},
			P3TriDegCGV{*this, 2,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2]
			},
			P3TriDegCGV{*this, 3,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3]
			},
			P3TriDegCGV{*this, 4,
				_r_m0[4], 
				_r_x0[4], 
				_r_x1[4], 
				_r_x2[4]
			},
			P3TriDegCGV{*this, 5,
				_r_m0[5], 
				_r_x0[5], 
				_r_x1[5], 
				_r_x2[5]
			},
			P3TriDegCGV{*this, 6,
				_r_m0[6], 
				_r_x0[6], 
				_r_x1[6], 
				_r_x2[6]
			},
			P3TriDegCGV{*this, 7,
				_r_m0[7], 
				_r_x0[7], 
				_r_x1[7], 
				_r_x2[7]
			},
		};
	}
	template<> std::array<P3TriDegCGV, P3TriDegCGV::schemes[1]> P3TriDegCGV::split_impl<1>() const {
		auto _r_m0 = subdiv_1_2p4_1p2(degenerateZeroed_m0());
		auto _r_x0 = subdiv_1_2p1_1p1(x0);
		auto _r_x1 = subdiv_1_2p1_1p1(x1);
		auto _r_x2 = subdiv_1_2p1_1p1(x2);
		return {
			P3TriDegCGV{*this, 8,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0]
			},
			P3TriDegCGV{*this, 9,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1]
			},
		};
	}
	template<> std::array<P3TriDegCGV, P3TriDegCGV::schemes[2]> P3TriDegCGV::split_impl<2>() const {
		auto _r_m0 = subdiv_2_2p4_1p2(degenerateZeroed_m0());
		auto _r_x0 = subdiv_2_2p1_1p1(x0);
		auto _r_x1 = subdiv_2_2p1_1p1(x1);
		auto _r_x2 = subdiv_2_2p1_1p1(x2);
		return {
			P3TriDegCGV{*this, 10,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0]
			},
			P3TriDegCGV{*this, 11,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1]
			},
			P3TriDegCGV{*this, 12,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2]
			},
			P3TriDegCGV{*this, 13,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3]
			},
		};
	}
	std::ostream &operator<<(std::ostream &out, const P3TriDegCGV &s) {
		for (unsigned i=0; i<s.numVertices; ++i) {
			out << s.getVertex(i) << ' ';
		}
		return out;
	}
}
