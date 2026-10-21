#include "../P3TetDegVal.hpp"
#include <stdexcept>
namespace miso {
	namespace {
		constexpr std::array<unsigned, 9> kDegenerateStateVertices = {0u, 8u, 8u, 4u, 4u, 2u, 2u, 1u, 1u};
		constexpr std::array<unsigned, 9> kDegenerateStateOrders = {0u, 1u, 2u, 1u, 2u, 1u, 2u, 1u, 2u};
		constexpr std::array<std::array<unsigned, 24>, 9> kDegenerateTransitions = {{
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0},
			{0, 2, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0, 2, 0, 0},
			{0, 0, 3, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 3, 0, 0, 3, 3, 3, 3, 0, 0, 3, 0},
			{0, 0, 4, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 4, 0, 0, 4, 4, 4, 4, 0, 0, 4, 0},
			{0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 5, 5, 5, 5},
			{0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 6, 6, 6, 6},
			{7, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0},
			{8, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0},
		}};
	}

	unsigned P3TetDegVal::degenerateStateFromVertices(unsigned vertexMask, unsigned order) {
		if (vertexMask == 0u) return 0u;
		for (unsigned s=0; s<9; ++s) {
			if (kDegenerateStateVertices[s] == vertexMask && kDegenerateStateOrders[s] == order) return s;
		}
		throw std::invalid_argument("No degenerate state for this vertex set and order");
	}
	unsigned P3TetDegVal::degenerateStateVertexMask(unsigned s) { return kDegenerateStateVertices[s]; }
	unsigned P3TetDegVal::degenerateStateOrder(unsigned s) { return kDegenerateStateOrders[s]; }
	unsigned P3TetDegVal::degenerateTransition(unsigned s, unsigned q) { return kDegenerateTransitions[s][q]; }

	namespace {
		constexpr std::array<unsigned, 9> kAlternateScheme = {0u, 0u, 2u, 0u, 3u, 0u, 4u, 0u, 1u};
		constexpr std::array<unsigned, 24> kAdvancesAlternation = {1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u};
	}
	unsigned P3TetDegVal::alternateScheme(unsigned s) { return kAlternateScheme[s]; }
	unsigned P3TetDegVal::effectiveScheme(unsigned s) const {
		if (s == 0u && (alternation & 1u)) {
			const unsigned a = kAlternateScheme[degenerateState];
			if (a) return a;
		}
		return s;
	}

	P3TetDegVal::P3TetDegVal(const P3TetDegVal &parent, unsigned q,
		const RealVector<84> &m0,
		const RealVector<4> &x0,
		const RealVector<4> &x1,
		const RealVector<4> &x2
	) :
	m0(m0),
	x0(x0),
	x1(x1),
	x2(x2)
	{ inherit(parent, q); }

	P3TetDegVal::P3TetDegVal(
		const RealVector<20> &px,
		const RealVector<20> &py,
		const RealVector<20> &pz
	) :
	m0(LB_3p6(CL_m0(px, py, pz))),
	x0{0, 0, 0, 1},
	x1{0, 0, 1, 0},
	x2{0, 1, 0, 0}
	{
		history = std::make_shared<SubdivHistory>();
	}

	void P3TetDegVal::inherit(const P3TetDegVal &parent, unsigned q) {
		depth = parent.depth + 1;
		history = std::make_shared<SubdivHistory>(q, parent.history);
		degenerateState = kDegenerateTransitions[parent.degenerateState][q];
		alternation = parent.alternation + kAdvancesAlternation[q];
	}
	RealInterval P3TetDegVal::inclusion(unsigned i) const {
		switch (i) {
			case 0:
			switch (degenerateState) {
				case 1: return m0.inclusionDegenerate<83>();
				case 2: return m0.inclusionDegenerate<80,81,82,83>();
				case 3: return m0.inclusionDegenerate<27>();
				case 4: return m0.inclusionDegenerate<25,26,27,48>();
				case 5: return m0.inclusionDegenerate<6>();
				case 6: return m0.inclusionDegenerate<5,6,12,33>();
				case 7: return m0.inclusionDegenerate<0>();
				case 8: return m0.inclusionDegenerate<0,1,7,28>();
				default: return m0.inclusion();
			}
			default: throw std::logic_error("Undefined inclusion function");
		}
	}
	static constexpr std::array<std::array<unsigned, 2>, 56> kPairs_m0_0 = {{{28u, 0u}, {29u, 1u}, {30u, 2u}, {31u, 3u}, {32u, 4u}, {33u, 5u}, {34u, 7u}, {35u, 8u}, {36u, 9u}, {37u, 10u}, {38u, 11u}, {39u, 13u}, {40u, 14u}, {41u, 15u}, {42u, 16u}, {43u, 18u}, {44u, 19u}, {45u, 20u}, {46u, 22u}, {47u, 23u}, {48u, 25u}, {49u, 28u}, {50u, 29u}, {51u, 30u}, {52u, 31u}, {53u, 32u}, {54u, 34u}, {55u, 35u}, {56u, 36u}, {57u, 37u}, {58u, 39u}, {59u, 40u}, {60u, 41u}, {61u, 43u}, {62u, 44u}, {63u, 46u}, {64u, 49u}, {65u, 50u}, {66u, 51u}, {67u, 52u}, {68u, 54u}, {69u, 55u}, {70u, 56u}, {71u, 58u}, {72u, 59u}, {73u, 61u}, {74u, 64u}, {75u, 65u}, {76u, 66u}, {77u, 68u}, {78u, 69u}, {79u, 71u}, {80u, 74u}, {81u, 75u}, {82u, 77u}, {83u, 80u}}};
	static constexpr std::array<std::array<unsigned, 2>, 56> kPairs_m0_1 = {{{7u, 0u}, {8u, 1u}, {9u, 2u}, {10u, 3u}, {11u, 4u}, {12u, 5u}, {13u, 7u}, {14u, 8u}, {15u, 9u}, {16u, 10u}, {17u, 11u}, {18u, 13u}, {19u, 14u}, {20u, 15u}, {21u, 16u}, {22u, 18u}, {23u, 19u}, {24u, 20u}, {25u, 22u}, {26u, 23u}, {27u, 25u}, {34u, 28u}, {35u, 29u}, {36u, 30u}, {37u, 31u}, {38u, 32u}, {39u, 34u}, {40u, 35u}, {41u, 36u}, {42u, 37u}, {43u, 39u}, {44u, 40u}, {45u, 41u}, {46u, 43u}, {47u, 44u}, {48u, 46u}, {54u, 49u}, {55u, 50u}, {56u, 51u}, {57u, 52u}, {58u, 54u}, {59u, 55u}, {60u, 56u}, {61u, 58u}, {62u, 59u}, {63u, 61u}, {68u, 64u}, {69u, 65u}, {70u, 66u}, {71u, 68u}, {72u, 69u}, {73u, 71u}, {77u, 74u}, {78u, 75u}, {79u, 77u}, {82u, 80u}}};
	static constexpr std::array<std::array<unsigned, 2>, 56> kPairs_m0_2 = {{{1u, 0u}, {2u, 1u}, {3u, 2u}, {4u, 3u}, {5u, 4u}, {6u, 5u}, {8u, 7u}, {9u, 8u}, {10u, 9u}, {11u, 10u}, {12u, 11u}, {14u, 13u}, {15u, 14u}, {16u, 15u}, {17u, 16u}, {19u, 18u}, {20u, 19u}, {21u, 20u}, {23u, 22u}, {24u, 23u}, {26u, 25u}, {29u, 28u}, {30u, 29u}, {31u, 30u}, {32u, 31u}, {33u, 32u}, {35u, 34u}, {36u, 35u}, {37u, 36u}, {38u, 37u}, {40u, 39u}, {41u, 40u}, {42u, 41u}, {44u, 43u}, {45u, 44u}, {47u, 46u}, {50u, 49u}, {51u, 50u}, {52u, 51u}, {53u, 52u}, {55u, 54u}, {56u, 55u}, {57u, 56u}, {59u, 58u}, {60u, 59u}, {62u, 61u}, {65u, 64u}, {66u, 65u}, {67u, 66u}, {69u, 68u}, {70u, 69u}, {72u, 71u}, {75u, 74u}, {76u, 75u}, {78u, 77u}, {81u, 80u}}};
	double P3TetDegVal::sampleError_m0() const {
		#ifdef MISO_FP_ARITHMETIC
		return 0.;
		#else
		const Real L00 = x0[3] - x0[0];
		const Real L01 = x0[2] - x0[0];
		const Real L02 = x0[1] - x0[0];
		const Real L10 = x1[3] - x1[0];
		const Real L11 = x1[2] - x1[0];
		const Real L12 = x1[1] - x1[0];
		const Real L20 = x2[3] - x2[0];
		const Real L21 = x2[2] - x2[0];
		const Real L22 = x2[1] - x2[0];
		const Real det = L00 * (L11 * L22 - L12 * L21) - L01 * (L10 * L22 - L12 * L20) + L02 * (L10 * L21 - L11 * L20);
		if (lower(det) <= 0. && upper(det) >= 0.) return infinity;
		const Real adet = abs(det);
		double vmax = 0.;
		vmax = std::max(vmax, upper(abs(x0[0])));
		vmax = std::max(vmax, upper(abs(x1[0])));
		vmax = std::max(vmax, upper(abs(x2[0])));
		vmax = std::max(vmax, upper(abs(x0[3])));
		vmax = std::max(vmax, upper(abs(x1[3])));
		vmax = std::max(vmax, upper(abs(x2[3])));
		vmax = std::max(vmax, upper(abs(x0[2])));
		vmax = std::max(vmax, upper(abs(x1[2])));
		vmax = std::max(vmax, upper(abs(x2[2])));
		vmax = std::max(vmax, upper(abs(x0[1])));
		vmax = std::max(vmax, upper(abs(x1[1])));
		vmax = std::max(vmax, upper(abs(x2[1])));
		const Real eps = Real(0x1.c000000000000p-50) * Real(vmax);
		Real sumdelta(0.);
		Real err(0.);
		const Real delta0 = (abs((L11 * L22 - L12 * L21)) + abs((L02 * L21 - L01 * L22)) + abs((L01 * L12 - L02 * L11))) / adet * eps;
		sumdelta = sumdelta + delta0;
		double D0 = 0.;
		for (const auto &p : kPairs_m0_0) D0 = std::max(D0, upper(abs(m0[p[0]] - m0[p[1]])));
		err = err + delta0 * Real(D0);
		const Real delta1 = (abs((L12 * L20 - L10 * L22)) + abs((L00 * L22 - L02 * L20)) + abs((L02 * L10 - L00 * L12))) / adet * eps;
		sumdelta = sumdelta + delta1;
		double D1 = 0.;
		for (const auto &p : kPairs_m0_1) D1 = std::max(D1, upper(abs(m0[p[0]] - m0[p[1]])));
		err = err + delta1 * Real(D1);
		const Real delta2 = (abs((L10 * L21 - L11 * L20)) + abs((L01 * L20 - L00 * L21)) + abs((L00 * L11 - L01 * L10))) / adet * eps;
		sumdelta = sumdelta + delta2;
		double D2 = 0.;
		for (const auto &p : kPairs_m0_2) D2 = std::max(D2, upper(abs(m0[p[0]] - m0[p[1]])));
		err = err + delta2 * Real(D2);
		if (!(upper(sumdelta) < 0x1.3964e5ab1b800p-9)) return infinity;
		return upper(Real(6.) * err);
		#endif
	}
	RealVector<P3TetDegVal::numSamples> P3TetDegVal::sampleLanes_m0() const {
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
	RealVector<P3TetDegVal::numSamples> P3TetDegVal::sample(unsigned i) const {
		switch (i) {
			case 0: {
				const RealVector<numSamples> _s_m0 = sampleLanes_m0();
				return _s_m0;
			}
			default: throw std::logic_error("Undefined sampling function");
		}
	}
	RealVector<84> P3TetDegVal::degenerateZeroed_m0() const {
		RealVector<84> _b = m0;
		switch (degenerateState) {
			case 1: _b[83] = Real(0); break;
			case 2: _b[80] = Real(0); _b[81] = Real(0); _b[82] = Real(0); _b[83] = Real(0); break;
			case 3: _b[27] = Real(0); break;
			case 4: _b[25] = Real(0); _b[26] = Real(0); _b[27] = Real(0); _b[48] = Real(0); break;
			case 5: _b[6] = Real(0); break;
			case 6: _b[5] = Real(0); _b[6] = Real(0); _b[12] = Real(0); _b[33] = Real(0); break;
			case 7: _b[0] = Real(0); break;
			case 8: _b[0] = Real(0); _b[1] = Real(0); _b[7] = Real(0); _b[28] = Real(0); break;
			default: break;
		}
		return _b;
	}
	template<> std::array<P3TetDegVal, P3TetDegVal::schemes[0]> P3TetDegVal::split_impl<0>() const {
		auto _r_m0 = subdiv_0_3p6(degenerateZeroed_m0());
		auto _r_x0 = subdiv_0_3p1(x0);
		auto _r_x1 = subdiv_0_3p1(x1);
		auto _r_x2 = subdiv_0_3p1(x2);
		return {
			P3TetDegVal{*this, 0,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0]
			},
			P3TetDegVal{*this, 1,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1]
			},
			P3TetDegVal{*this, 2,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2]
			},
			P3TetDegVal{*this, 3,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3]
			},
			P3TetDegVal{*this, 4,
				_r_m0[4], 
				_r_x0[4], 
				_r_x1[4], 
				_r_x2[4]
			},
			P3TetDegVal{*this, 5,
				_r_m0[5], 
				_r_x0[5], 
				_r_x1[5], 
				_r_x2[5]
			},
			P3TetDegVal{*this, 6,
				_r_m0[6], 
				_r_x0[6], 
				_r_x1[6], 
				_r_x2[6]
			},
			P3TetDegVal{*this, 7,
				_r_m0[7], 
				_r_x0[7], 
				_r_x1[7], 
				_r_x2[7]
			},
		};
	}
	template<> std::array<P3TetDegVal, P3TetDegVal::schemes[1]> P3TetDegVal::split_impl<1>() const {
		auto _r_m0 = subdiv_1_3p6(degenerateZeroed_m0());
		auto _r_x0 = subdiv_1_3p1(x0);
		auto _r_x1 = subdiv_1_3p1(x1);
		auto _r_x2 = subdiv_1_3p1(x2);
		return {
			P3TetDegVal{*this, 8,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0]
			},
			P3TetDegVal{*this, 9,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1]
			},
			P3TetDegVal{*this, 10,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2]
			},
			P3TetDegVal{*this, 11,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3]
			},
		};
	}
	template<> std::array<P3TetDegVal, P3TetDegVal::schemes[2]> P3TetDegVal::split_impl<2>() const {
		auto _r_m0 = subdiv_2_3p6(degenerateZeroed_m0());
		auto _r_x0 = subdiv_2_3p1(x0);
		auto _r_x1 = subdiv_2_3p1(x1);
		auto _r_x2 = subdiv_2_3p1(x2);
		return {
			P3TetDegVal{*this, 12,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0]
			},
			P3TetDegVal{*this, 13,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1]
			},
			P3TetDegVal{*this, 14,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2]
			},
			P3TetDegVal{*this, 15,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3]
			},
		};
	}
	template<> std::array<P3TetDegVal, P3TetDegVal::schemes[3]> P3TetDegVal::split_impl<3>() const {
		auto _r_m0 = subdiv_3_3p6(degenerateZeroed_m0());
		auto _r_x0 = subdiv_3_3p1(x0);
		auto _r_x1 = subdiv_3_3p1(x1);
		auto _r_x2 = subdiv_3_3p1(x2);
		return {
			P3TetDegVal{*this, 16,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0]
			},
			P3TetDegVal{*this, 17,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1]
			},
			P3TetDegVal{*this, 18,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2]
			},
			P3TetDegVal{*this, 19,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3]
			},
		};
	}
	template<> std::array<P3TetDegVal, P3TetDegVal::schemes[4]> P3TetDegVal::split_impl<4>() const {
		auto _r_m0 = subdiv_4_3p6(degenerateZeroed_m0());
		auto _r_x0 = subdiv_4_3p1(x0);
		auto _r_x1 = subdiv_4_3p1(x1);
		auto _r_x2 = subdiv_4_3p1(x2);
		return {
			P3TetDegVal{*this, 20,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0]
			},
			P3TetDegVal{*this, 21,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1]
			},
			P3TetDegVal{*this, 22,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2]
			},
			P3TetDegVal{*this, 23,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3]
			},
		};
	}
	std::ostream &operator<<(std::ostream &out, const P3TetDegVal &s) {
		for (unsigned i=0; i<s.numVertices; ++i) {
			out << s.getVertex(i) << ' ';
		}
		return out;
	}
}
