#include "../P3TetDegCGV.hpp"
#include <stdexcept>
namespace miso {
	namespace {
		constexpr std::array<unsigned, 9> kDegenerateStateVertices = {0u, 192u, 192u, 48u, 48u, 12u, 12u, 3u, 3u};
		constexpr std::array<unsigned, 9> kDegenerateStateOrders = {0u, 1u, 2u, 1u, 2u, 1u, 2u, 1u, 2u};
		constexpr std::array<std::array<unsigned, 42>, 9> kDegenerateTransitions = {{
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
			{0, 0, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 0, 0, 0, 2, 2, 2, 2, 0, 2, 0, 0, 0, 2, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 3, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 0, 3, 0, 0, 0, 3, 0, 0, 3, 3, 3, 3, 0, 0, 3, 0, 0, 0, 3, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 0, 4, 0, 0, 0, 4, 0, 0, 4, 4, 4, 4, 0, 0, 4, 0, 0, 0, 4, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0, 5, 5, 0, 0, 5, 0, 0, 0, 5, 0, 0, 0, 5, 0, 5, 5, 5, 5, 0, 0, 0, 5, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 0, 0, 0, 0, 0, 0, 6, 6, 0, 0, 6, 0, 0, 0, 6, 0, 0, 0, 6, 0, 6, 6, 6, 6, 0, 0, 0, 6, 0, 0, 0, 0},
			{7, 7, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 7, 7, 7, 7, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0},
			{8, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 8, 8, 8, 8, 8, 8, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 8, 0, 0, 0, 0, 0, 0, 0},
		}};
	}

	unsigned P3TetDegCGV::degenerateStateFromVertices(unsigned vertexMask, unsigned order) {
		if (vertexMask == 0u) return 0u;
		for (unsigned s=0; s<9; ++s) {
			if (kDegenerateStateVertices[s] == vertexMask && kDegenerateStateOrders[s] == order) return s;
		}
		throw std::invalid_argument("No degenerate state for this vertex set and order");
	}
	unsigned P3TetDegCGV::degenerateStateVertexMask(unsigned s) { return kDegenerateStateVertices[s]; }
	unsigned P3TetDegCGV::degenerateStateOrder(unsigned s) { return kDegenerateStateOrders[s]; }
	unsigned P3TetDegCGV::degenerateTransition(unsigned s, unsigned q) { return kDegenerateTransitions[s][q]; }

	namespace {
		constexpr std::array<unsigned, 9> kAlternateScheme = {6u, 6u, 3u, 6u, 4u, 6u, 5u, 6u, 2u};
		constexpr std::array<unsigned, 42> kAdvancesAlternation = {1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 0u, 0u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u, 1u};
	}
	unsigned P3TetDegCGV::alternateScheme(unsigned s) { return kAlternateScheme[s]; }
	unsigned P3TetDegCGV::effectiveScheme(unsigned s) const {
		if (s == 0u && (alternation & 1u)) {
			const unsigned a = kAlternateScheme[degenerateState];
			if (a) return a;
		}
		return s;
	}

	P3TetDegCGV::P3TetDegCGV(const P3TetDegCGV &parent, unsigned q,
		const RealVector<336> &m0,
		const RealVector<8> &x0,
		const RealVector<8> &x1,
		const RealVector<8> &x2,
		const RealVector<8> &x3
	) :
	m0(m0),
	x0(x0),
	x1(x1),
	x2(x2),
	x3(x3)
	{ inherit(parent, q); }

	P3TetDegCGV::P3TetDegCGV(
		const RealVector<20> &p0x,
		const RealVector<20> &p0y,
		const RealVector<20> &p0z,
		const RealVector<20> &p1x,
		const RealVector<20> &p1y,
		const RealVector<20> &p1z
	) :
	m0(LB_3p6_1p3(CL_m0(p0x, p0y, p0z, p1x, p1y, p1z))),
	x0{0, 0, 0, 0, 0, 0, 1, 1},
	x1{0, 0, 0, 0, 1, 1, 0, 0},
	x2{0, 0, 1, 1, 0, 0, 0, 0},
	x3{0, 1, 0, 1, 0, 1, 0, 1}
	{
		history = std::make_shared<SubdivHistory>();
	}

	void P3TetDegCGV::inherit(const P3TetDegCGV &parent, unsigned q) {
		depth = parent.depth + 1;
		history = std::make_shared<SubdivHistory>(q, parent.history);
		degenerateState = kDegenerateTransitions[parent.degenerateState][q];
		alternation = parent.alternation + kAdvancesAlternation[q];
	}
	RealInterval P3TetDegCGV::inclusion(unsigned i) const {
		switch (i) {
			case 0:
			switch (degenerateState) {
				case 1: return m0.inclusionDegenerate<332,333,334,335>();
				case 2: return m0.inclusionDegenerate<320,321,322,323,324,325,326,327,328,329,330,331,332,333,334,335>();
				case 3: return m0.inclusionDegenerate<108,109,110,111>();
				case 4: return m0.inclusionDegenerate<100,101,102,103,104,105,106,107,108,109,110,111,192,193,194,195>();
				case 5: return m0.inclusionDegenerate<24,25,26,27>();
				case 6: return m0.inclusionDegenerate<20,21,22,23,24,25,26,27,48,49,50,51,132,133,134,135>();
				case 7: return m0.inclusionDegenerate<0,1,2,3>();
				case 8: return m0.inclusionDegenerate<0,1,2,3,4,5,6,7,28,29,30,31,112,113,114,115>();
				default: return m0.inclusion();
			}
			case 1: return x3.inclusion();
			default: throw std::logic_error("Undefined inclusion function");
		}
	}
	static constexpr std::array<std::array<unsigned, 2>, 224> kPairs_m0_0 = {{{112u, 0u}, {113u, 1u}, {114u, 2u}, {115u, 3u}, {116u, 4u}, {117u, 5u}, {118u, 6u}, {119u, 7u}, {120u, 8u}, {121u, 9u}, {122u, 10u}, {123u, 11u}, {124u, 12u}, {125u, 13u}, {126u, 14u}, {127u, 15u}, {128u, 16u}, {129u, 17u}, {130u, 18u}, {131u, 19u}, {132u, 20u}, {133u, 21u}, {134u, 22u}, {135u, 23u}, {136u, 28u}, {137u, 29u}, {138u, 30u}, {139u, 31u}, {140u, 32u}, {141u, 33u}, {142u, 34u}, {143u, 35u}, {144u, 36u}, {145u, 37u}, {146u, 38u}, {147u, 39u}, {148u, 40u}, {149u, 41u}, {150u, 42u}, {151u, 43u}, {152u, 44u}, {153u, 45u}, {154u, 46u}, {155u, 47u}, {156u, 52u}, {157u, 53u}, {158u, 54u}, {159u, 55u}, {160u, 56u}, {161u, 57u}, {162u, 58u}, {163u, 59u}, {164u, 60u}, {165u, 61u}, {166u, 62u}, {167u, 63u}, {168u, 64u}, {169u, 65u}, {170u, 66u}, {171u, 67u}, {172u, 72u}, {173u, 73u}, {174u, 74u}, {175u, 75u}, {176u, 76u}, {177u, 77u}, {178u, 78u}, {179u, 79u}, {180u, 80u}, {181u, 81u}, {182u, 82u}, {183u, 83u}, {184u, 88u}, {185u, 89u}, {186u, 90u}, {187u, 91u}, {188u, 92u}, {189u, 93u}, {190u, 94u}, {191u, 95u}, {192u, 100u}, {193u, 101u}, {194u, 102u}, {195u, 103u}, {196u, 112u}, {197u, 113u}, {198u, 114u}, {199u, 115u}, {200u, 116u}, {201u, 117u}, {202u, 118u}, {203u, 119u}, {204u, 120u}, {205u, 121u}, {206u, 122u}, {207u, 123u}, {208u, 124u}, {209u, 125u}, {210u, 126u}, {211u, 127u}, {212u, 128u}, {213u, 129u}, {214u, 130u}, {215u, 131u}, {216u, 136u}, {217u, 137u}, {218u, 138u}, {219u, 139u}, {220u, 140u}, {221u, 141u}, {222u, 142u}, {223u, 143u}, {224u, 144u}, {225u, 145u}, {226u, 146u}, {227u, 147u}, {228u, 148u}, {229u, 149u}, {230u, 150u}, {231u, 151u}, {232u, 156u}, {233u, 157u}, {234u, 158u}, {235u, 159u}, {236u, 160u}, {237u, 161u}, {238u, 162u}, {239u, 163u}, {240u, 164u}, {241u, 165u}, {242u, 166u}, {243u, 167u}, {244u, 172u}, {245u, 173u}, {246u, 174u}, {247u, 175u}, {248u, 176u}, {249u, 177u}, {250u, 178u}, {251u, 179u}, {252u, 184u}, {253u, 185u}, {254u, 186u}, {255u, 187u}, {256u, 196u}, {257u, 197u}, {258u, 198u}, {259u, 199u}, {260u, 200u}, {261u, 201u}, {262u, 202u}, {263u, 203u}, {264u, 204u}, {265u, 205u}, {266u, 206u}, {267u, 207u}, {268u, 208u}, {269u, 209u}, {270u, 210u}, {271u, 211u}, {272u, 216u}, {273u, 217u}, {274u, 218u}, {275u, 219u}, {276u, 220u}, {277u, 221u}, {278u, 222u}, {279u, 223u}, {280u, 224u}, {281u, 225u}, {282u, 226u}, {283u, 227u}, {284u, 232u}, {285u, 233u}, {286u, 234u}, {287u, 235u}, {288u, 236u}, {289u, 237u}, {290u, 238u}, {291u, 239u}, {292u, 244u}, {293u, 245u}, {294u, 246u}, {295u, 247u}, {296u, 256u}, {297u, 257u}, {298u, 258u}, {299u, 259u}, {300u, 260u}, {301u, 261u}, {302u, 262u}, {303u, 263u}, {304u, 264u}, {305u, 265u}, {306u, 266u}, {307u, 267u}, {308u, 272u}, {309u, 273u}, {310u, 274u}, {311u, 275u}, {312u, 276u}, {313u, 277u}, {314u, 278u}, {315u, 279u}, {316u, 284u}, {317u, 285u}, {318u, 286u}, {319u, 287u}, {320u, 296u}, {321u, 297u}, {322u, 298u}, {323u, 299u}, {324u, 300u}, {325u, 301u}, {326u, 302u}, {327u, 303u}, {328u, 308u}, {329u, 309u}, {330u, 310u}, {331u, 311u}, {332u, 320u}, {333u, 321u}, {334u, 322u}, {335u, 323u}}};
	static constexpr std::array<std::array<unsigned, 2>, 224> kPairs_m0_1 = {{{28u, 0u}, {29u, 1u}, {30u, 2u}, {31u, 3u}, {32u, 4u}, {33u, 5u}, {34u, 6u}, {35u, 7u}, {36u, 8u}, {37u, 9u}, {38u, 10u}, {39u, 11u}, {40u, 12u}, {41u, 13u}, {42u, 14u}, {43u, 15u}, {44u, 16u}, {45u, 17u}, {46u, 18u}, {47u, 19u}, {48u, 20u}, {49u, 21u}, {50u, 22u}, {51u, 23u}, {52u, 28u}, {53u, 29u}, {54u, 30u}, {55u, 31u}, {56u, 32u}, {57u, 33u}, {58u, 34u}, {59u, 35u}, {60u, 36u}, {61u, 37u}, {62u, 38u}, {63u, 39u}, {64u, 40u}, {65u, 41u}, {66u, 42u}, {67u, 43u}, {68u, 44u}, {69u, 45u}, {70u, 46u}, {71u, 47u}, {72u, 52u}, {73u, 53u}, {74u, 54u}, {75u, 55u}, {76u, 56u}, {77u, 57u}, {78u, 58u}, {79u, 59u}, {80u, 60u}, {81u, 61u}, {82u, 62u}, {83u, 63u}, {84u, 64u}, {85u, 65u}, {86u, 66u}, {87u, 67u}, {88u, 72u}, {89u, 73u}, {90u, 74u}, {91u, 75u}, {92u, 76u}, {93u, 77u}, {94u, 78u}, {95u, 79u}, {96u, 80u}, {97u, 81u}, {98u, 82u}, {99u, 83u}, {100u, 88u}, {101u, 89u}, {102u, 90u}, {103u, 91u}, {104u, 92u}, {105u, 93u}, {106u, 94u}, {107u, 95u}, {108u, 100u}, {109u, 101u}, {110u, 102u}, {111u, 103u}, {136u, 112u}, {137u, 113u}, {138u, 114u}, {139u, 115u}, {140u, 116u}, {141u, 117u}, {142u, 118u}, {143u, 119u}, {144u, 120u}, {145u, 121u}, {146u, 122u}, {147u, 123u}, {148u, 124u}, {149u, 125u}, {150u, 126u}, {151u, 127u}, {152u, 128u}, {153u, 129u}, {154u, 130u}, {155u, 131u}, {156u, 136u}, {157u, 137u}, {158u, 138u}, {159u, 139u}, {160u, 140u}, {161u, 141u}, {162u, 142u}, {163u, 143u}, {164u, 144u}, {165u, 145u}, {166u, 146u}, {167u, 147u}, {168u, 148u}, {169u, 149u}, {170u, 150u}, {171u, 151u}, {172u, 156u}, {173u, 157u}, {174u, 158u}, {175u, 159u}, {176u, 160u}, {177u, 161u}, {178u, 162u}, {179u, 163u}, {180u, 164u}, {181u, 165u}, {182u, 166u}, {183u, 167u}, {184u, 172u}, {185u, 173u}, {186u, 174u}, {187u, 175u}, {188u, 176u}, {189u, 177u}, {190u, 178u}, {191u, 179u}, {192u, 184u}, {193u, 185u}, {194u, 186u}, {195u, 187u}, {216u, 196u}, {217u, 197u}, {218u, 198u}, {219u, 199u}, {220u, 200u}, {221u, 201u}, {222u, 202u}, {223u, 203u}, {224u, 204u}, {225u, 205u}, {226u, 206u}, {227u, 207u}, {228u, 208u}, {229u, 209u}, {230u, 210u}, {231u, 211u}, {232u, 216u}, {233u, 217u}, {234u, 218u}, {235u, 219u}, {236u, 220u}, {237u, 221u}, {238u, 222u}, {239u, 223u}, {240u, 224u}, {241u, 225u}, {242u, 226u}, {243u, 227u}, {244u, 232u}, {245u, 233u}, {246u, 234u}, {247u, 235u}, {248u, 236u}, {249u, 237u}, {250u, 238u}, {251u, 239u}, {252u, 244u}, {253u, 245u}, {254u, 246u}, {255u, 247u}, {272u, 256u}, {273u, 257u}, {274u, 258u}, {275u, 259u}, {276u, 260u}, {277u, 261u}, {278u, 262u}, {279u, 263u}, {280u, 264u}, {281u, 265u}, {282u, 266u}, {283u, 267u}, {284u, 272u}, {285u, 273u}, {286u, 274u}, {287u, 275u}, {288u, 276u}, {289u, 277u}, {290u, 278u}, {291u, 279u}, {292u, 284u}, {293u, 285u}, {294u, 286u}, {295u, 287u}, {308u, 296u}, {309u, 297u}, {310u, 298u}, {311u, 299u}, {312u, 300u}, {313u, 301u}, {314u, 302u}, {315u, 303u}, {316u, 308u}, {317u, 309u}, {318u, 310u}, {319u, 311u}, {328u, 320u}, {329u, 321u}, {330u, 322u}, {331u, 323u}}};
	static constexpr std::array<std::array<unsigned, 2>, 224> kPairs_m0_2 = {{{4u, 0u}, {5u, 1u}, {6u, 2u}, {7u, 3u}, {8u, 4u}, {9u, 5u}, {10u, 6u}, {11u, 7u}, {12u, 8u}, {13u, 9u}, {14u, 10u}, {15u, 11u}, {16u, 12u}, {17u, 13u}, {18u, 14u}, {19u, 15u}, {20u, 16u}, {21u, 17u}, {22u, 18u}, {23u, 19u}, {24u, 20u}, {25u, 21u}, {26u, 22u}, {27u, 23u}, {32u, 28u}, {33u, 29u}, {34u, 30u}, {35u, 31u}, {36u, 32u}, {37u, 33u}, {38u, 34u}, {39u, 35u}, {40u, 36u}, {41u, 37u}, {42u, 38u}, {43u, 39u}, {44u, 40u}, {45u, 41u}, {46u, 42u}, {47u, 43u}, {48u, 44u}, {49u, 45u}, {50u, 46u}, {51u, 47u}, {56u, 52u}, {57u, 53u}, {58u, 54u}, {59u, 55u}, {60u, 56u}, {61u, 57u}, {62u, 58u}, {63u, 59u}, {64u, 60u}, {65u, 61u}, {66u, 62u}, {67u, 63u}, {68u, 64u}, {69u, 65u}, {70u, 66u}, {71u, 67u}, {76u, 72u}, {77u, 73u}, {78u, 74u}, {79u, 75u}, {80u, 76u}, {81u, 77u}, {82u, 78u}, {83u, 79u}, {84u, 80u}, {85u, 81u}, {86u, 82u}, {87u, 83u}, {92u, 88u}, {93u, 89u}, {94u, 90u}, {95u, 91u}, {96u, 92u}, {97u, 93u}, {98u, 94u}, {99u, 95u}, {104u, 100u}, {105u, 101u}, {106u, 102u}, {107u, 103u}, {116u, 112u}, {117u, 113u}, {118u, 114u}, {119u, 115u}, {120u, 116u}, {121u, 117u}, {122u, 118u}, {123u, 119u}, {124u, 120u}, {125u, 121u}, {126u, 122u}, {127u, 123u}, {128u, 124u}, {129u, 125u}, {130u, 126u}, {131u, 127u}, {132u, 128u}, {133u, 129u}, {134u, 130u}, {135u, 131u}, {140u, 136u}, {141u, 137u}, {142u, 138u}, {143u, 139u}, {144u, 140u}, {145u, 141u}, {146u, 142u}, {147u, 143u}, {148u, 144u}, {149u, 145u}, {150u, 146u}, {151u, 147u}, {152u, 148u}, {153u, 149u}, {154u, 150u}, {155u, 151u}, {160u, 156u}, {161u, 157u}, {162u, 158u}, {163u, 159u}, {164u, 160u}, {165u, 161u}, {166u, 162u}, {167u, 163u}, {168u, 164u}, {169u, 165u}, {170u, 166u}, {171u, 167u}, {176u, 172u}, {177u, 173u}, {178u, 174u}, {179u, 175u}, {180u, 176u}, {181u, 177u}, {182u, 178u}, {183u, 179u}, {188u, 184u}, {189u, 185u}, {190u, 186u}, {191u, 187u}, {200u, 196u}, {201u, 197u}, {202u, 198u}, {203u, 199u}, {204u, 200u}, {205u, 201u}, {206u, 202u}, {207u, 203u}, {208u, 204u}, {209u, 205u}, {210u, 206u}, {211u, 207u}, {212u, 208u}, {213u, 209u}, {214u, 210u}, {215u, 211u}, {220u, 216u}, {221u, 217u}, {222u, 218u}, {223u, 219u}, {224u, 220u}, {225u, 221u}, {226u, 222u}, {227u, 223u}, {228u, 224u}, {229u, 225u}, {230u, 226u}, {231u, 227u}, {236u, 232u}, {237u, 233u}, {238u, 234u}, {239u, 235u}, {240u, 236u}, {241u, 237u}, {242u, 238u}, {243u, 239u}, {248u, 244u}, {249u, 245u}, {250u, 246u}, {251u, 247u}, {260u, 256u}, {261u, 257u}, {262u, 258u}, {263u, 259u}, {264u, 260u}, {265u, 261u}, {266u, 262u}, {267u, 263u}, {268u, 264u}, {269u, 265u}, {270u, 266u}, {271u, 267u}, {276u, 272u}, {277u, 273u}, {278u, 274u}, {279u, 275u}, {280u, 276u}, {281u, 277u}, {282u, 278u}, {283u, 279u}, {288u, 284u}, {289u, 285u}, {290u, 286u}, {291u, 287u}, {300u, 296u}, {301u, 297u}, {302u, 298u}, {303u, 299u}, {304u, 300u}, {305u, 301u}, {306u, 302u}, {307u, 303u}, {312u, 308u}, {313u, 309u}, {314u, 310u}, {315u, 311u}, {324u, 320u}, {325u, 321u}, {326u, 322u}, {327u, 323u}}};
	double P3TetDegCGV::sampleError_m0() const {
		#ifdef MISO_FP_ARITHMETIC
		return 0.;
		#else
		const Real L00 = x0[6] - x0[0];
		const Real L01 = x0[4] - x0[0];
		const Real L02 = x0[2] - x0[0];
		const Real L10 = x1[6] - x1[0];
		const Real L11 = x1[4] - x1[0];
		const Real L12 = x1[2] - x1[0];
		const Real L20 = x2[6] - x2[0];
		const Real L21 = x2[4] - x2[0];
		const Real L22 = x2[2] - x2[0];
		const Real det = L00 * (L11 * L22 - L12 * L21) - L01 * (L10 * L22 - L12 * L20) + L02 * (L10 * L21 - L11 * L20);
		if (lower(det) <= 0. && upper(det) >= 0.) return infinity;
		const Real adet = abs(det);
		double vmax = 0.;
		vmax = std::max(vmax, upper(abs(x0[0])));
		vmax = std::max(vmax, upper(abs(x1[0])));
		vmax = std::max(vmax, upper(abs(x2[0])));
		vmax = std::max(vmax, upper(abs(x0[6])));
		vmax = std::max(vmax, upper(abs(x1[6])));
		vmax = std::max(vmax, upper(abs(x2[6])));
		vmax = std::max(vmax, upper(abs(x0[4])));
		vmax = std::max(vmax, upper(abs(x1[4])));
		vmax = std::max(vmax, upper(abs(x2[4])));
		vmax = std::max(vmax, upper(abs(x0[2])));
		vmax = std::max(vmax, upper(abs(x1[2])));
		vmax = std::max(vmax, upper(abs(x2[2])));
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
	RealVector<P3TetDegCGV::numSamples> P3TetDegCGV::sampleLanes_m0() const {
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
	RealVector<P3TetDegCGV::numSamples> P3TetDegCGV::sample(unsigned i) const {
		switch (i) {
			case 0: {
				const RealVector<numSamples> _s_m0 = sampleLanes_m0();
				return _s_m0;
			}
			case 1: {
				const RealVector<numSamples> _s_x3 = {x3[0], x3[1], x3[0], x3[1], x3[0], x3[1], x3[0], x3[1], x3[0], x3[1], x3[0], x3[1], x3[0], x3[1], x3[0], x3[1], x3[0], x3[1], x3[0], x3[1], x3[0], x3[1]};
				return _s_x3;
			}
			default: throw std::logic_error("Undefined sampling function");
		}
	}
	RealVector<336> P3TetDegCGV::degenerateZeroed_m0() const {
		RealVector<336> _b = m0;
		switch (degenerateState) {
			case 1: _b[332] = Real(0); _b[333] = Real(0); _b[334] = Real(0); _b[335] = Real(0); break;
			case 2: _b[320] = Real(0); _b[321] = Real(0); _b[322] = Real(0); _b[323] = Real(0); _b[324] = Real(0); _b[325] = Real(0); _b[326] = Real(0); _b[327] = Real(0); _b[328] = Real(0); _b[329] = Real(0); _b[330] = Real(0); _b[331] = Real(0); _b[332] = Real(0); _b[333] = Real(0); _b[334] = Real(0); _b[335] = Real(0); break;
			case 3: _b[108] = Real(0); _b[109] = Real(0); _b[110] = Real(0); _b[111] = Real(0); break;
			case 4: _b[100] = Real(0); _b[101] = Real(0); _b[102] = Real(0); _b[103] = Real(0); _b[104] = Real(0); _b[105] = Real(0); _b[106] = Real(0); _b[107] = Real(0); _b[108] = Real(0); _b[109] = Real(0); _b[110] = Real(0); _b[111] = Real(0); _b[192] = Real(0); _b[193] = Real(0); _b[194] = Real(0); _b[195] = Real(0); break;
			case 5: _b[24] = Real(0); _b[25] = Real(0); _b[26] = Real(0); _b[27] = Real(0); break;
			case 6: _b[20] = Real(0); _b[21] = Real(0); _b[22] = Real(0); _b[23] = Real(0); _b[24] = Real(0); _b[25] = Real(0); _b[26] = Real(0); _b[27] = Real(0); _b[48] = Real(0); _b[49] = Real(0); _b[50] = Real(0); _b[51] = Real(0); _b[132] = Real(0); _b[133] = Real(0); _b[134] = Real(0); _b[135] = Real(0); break;
			case 7: _b[0] = Real(0); _b[1] = Real(0); _b[2] = Real(0); _b[3] = Real(0); break;
			case 8: _b[0] = Real(0); _b[1] = Real(0); _b[2] = Real(0); _b[3] = Real(0); _b[4] = Real(0); _b[5] = Real(0); _b[6] = Real(0); _b[7] = Real(0); _b[28] = Real(0); _b[29] = Real(0); _b[30] = Real(0); _b[31] = Real(0); _b[112] = Real(0); _b[113] = Real(0); _b[114] = Real(0); _b[115] = Real(0); break;
			default: break;
		}
		return _b;
	}
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[0]> P3TetDegCGV::split_impl<0>() const {
		auto _r_m0 = subdiv_0_3p6_1p3(degenerateZeroed_m0());
		auto _r_x0 = subdiv_0_3p1_1p1(x0);
		auto _r_x1 = subdiv_0_3p1_1p1(x1);
		auto _r_x2 = subdiv_0_3p1_1p1(x2);
		auto _r_x3 = subdiv_0_3p1_1p1(x3);
		return {
			P3TetDegCGV{*this, 0,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0], 
				_r_x3[0]
			},
			P3TetDegCGV{*this, 1,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1], 
				_r_x3[1]
			},
			P3TetDegCGV{*this, 2,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2], 
				_r_x3[2]
			},
			P3TetDegCGV{*this, 3,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3], 
				_r_x3[3]
			},
			P3TetDegCGV{*this, 4,
				_r_m0[4], 
				_r_x0[4], 
				_r_x1[4], 
				_r_x2[4], 
				_r_x3[4]
			},
			P3TetDegCGV{*this, 5,
				_r_m0[5], 
				_r_x0[5], 
				_r_x1[5], 
				_r_x2[5], 
				_r_x3[5]
			},
			P3TetDegCGV{*this, 6,
				_r_m0[6], 
				_r_x0[6], 
				_r_x1[6], 
				_r_x2[6], 
				_r_x3[6]
			},
			P3TetDegCGV{*this, 7,
				_r_m0[7], 
				_r_x0[7], 
				_r_x1[7], 
				_r_x2[7], 
				_r_x3[7]
			},
			P3TetDegCGV{*this, 8,
				_r_m0[8], 
				_r_x0[8], 
				_r_x1[8], 
				_r_x2[8], 
				_r_x3[8]
			},
			P3TetDegCGV{*this, 9,
				_r_m0[9], 
				_r_x0[9], 
				_r_x1[9], 
				_r_x2[9], 
				_r_x3[9]
			},
			P3TetDegCGV{*this, 10,
				_r_m0[10], 
				_r_x0[10], 
				_r_x1[10], 
				_r_x2[10], 
				_r_x3[10]
			},
			P3TetDegCGV{*this, 11,
				_r_m0[11], 
				_r_x0[11], 
				_r_x1[11], 
				_r_x2[11], 
				_r_x3[11]
			},
			P3TetDegCGV{*this, 12,
				_r_m0[12], 
				_r_x0[12], 
				_r_x1[12], 
				_r_x2[12], 
				_r_x3[12]
			},
			P3TetDegCGV{*this, 13,
				_r_m0[13], 
				_r_x0[13], 
				_r_x1[13], 
				_r_x2[13], 
				_r_x3[13]
			},
			P3TetDegCGV{*this, 14,
				_r_m0[14], 
				_r_x0[14], 
				_r_x1[14], 
				_r_x2[14], 
				_r_x3[14]
			},
			P3TetDegCGV{*this, 15,
				_r_m0[15], 
				_r_x0[15], 
				_r_x1[15], 
				_r_x2[15], 
				_r_x3[15]
			},
		};
	}
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[1]> P3TetDegCGV::split_impl<1>() const {
		auto _r_m0 = subdiv_1_3p6_1p3(degenerateZeroed_m0());
		auto _r_x0 = subdiv_1_3p1_1p1(x0);
		auto _r_x1 = subdiv_1_3p1_1p1(x1);
		auto _r_x2 = subdiv_1_3p1_1p1(x2);
		auto _r_x3 = subdiv_1_3p1_1p1(x3);
		return {
			P3TetDegCGV{*this, 16,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0], 
				_r_x3[0]
			},
			P3TetDegCGV{*this, 17,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1], 
				_r_x3[1]
			},
		};
	}
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[2]> P3TetDegCGV::split_impl<2>() const {
		auto _r_m0 = subdiv_2_3p6_1p3(degenerateZeroed_m0());
		auto _r_x0 = subdiv_2_3p1_1p1(x0);
		auto _r_x1 = subdiv_2_3p1_1p1(x1);
		auto _r_x2 = subdiv_2_3p1_1p1(x2);
		auto _r_x3 = subdiv_2_3p1_1p1(x3);
		return {
			P3TetDegCGV{*this, 18,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0], 
				_r_x3[0]
			},
			P3TetDegCGV{*this, 19,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1], 
				_r_x3[1]
			},
			P3TetDegCGV{*this, 20,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2], 
				_r_x3[2]
			},
			P3TetDegCGV{*this, 21,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3], 
				_r_x3[3]
			},
		};
	}
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[3]> P3TetDegCGV::split_impl<3>() const {
		auto _r_m0 = subdiv_3_3p6_1p3(degenerateZeroed_m0());
		auto _r_x0 = subdiv_3_3p1_1p1(x0);
		auto _r_x1 = subdiv_3_3p1_1p1(x1);
		auto _r_x2 = subdiv_3_3p1_1p1(x2);
		auto _r_x3 = subdiv_3_3p1_1p1(x3);
		return {
			P3TetDegCGV{*this, 22,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0], 
				_r_x3[0]
			},
			P3TetDegCGV{*this, 23,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1], 
				_r_x3[1]
			},
			P3TetDegCGV{*this, 24,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2], 
				_r_x3[2]
			},
			P3TetDegCGV{*this, 25,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3], 
				_r_x3[3]
			},
		};
	}
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[4]> P3TetDegCGV::split_impl<4>() const {
		auto _r_m0 = subdiv_4_3p6_1p3(degenerateZeroed_m0());
		auto _r_x0 = subdiv_4_3p1_1p1(x0);
		auto _r_x1 = subdiv_4_3p1_1p1(x1);
		auto _r_x2 = subdiv_4_3p1_1p1(x2);
		auto _r_x3 = subdiv_4_3p1_1p1(x3);
		return {
			P3TetDegCGV{*this, 26,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0], 
				_r_x3[0]
			},
			P3TetDegCGV{*this, 27,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1], 
				_r_x3[1]
			},
			P3TetDegCGV{*this, 28,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2], 
				_r_x3[2]
			},
			P3TetDegCGV{*this, 29,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3], 
				_r_x3[3]
			},
		};
	}
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[5]> P3TetDegCGV::split_impl<5>() const {
		auto _r_m0 = subdiv_5_3p6_1p3(degenerateZeroed_m0());
		auto _r_x0 = subdiv_5_3p1_1p1(x0);
		auto _r_x1 = subdiv_5_3p1_1p1(x1);
		auto _r_x2 = subdiv_5_3p1_1p1(x2);
		auto _r_x3 = subdiv_5_3p1_1p1(x3);
		return {
			P3TetDegCGV{*this, 30,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0], 
				_r_x3[0]
			},
			P3TetDegCGV{*this, 31,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1], 
				_r_x3[1]
			},
			P3TetDegCGV{*this, 32,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2], 
				_r_x3[2]
			},
			P3TetDegCGV{*this, 33,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3], 
				_r_x3[3]
			},
		};
	}
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[6]> P3TetDegCGV::split_impl<6>() const {
		auto _r_m0 = subdiv_6_3p6_1p3(degenerateZeroed_m0());
		auto _r_x0 = subdiv_6_3p1_1p1(x0);
		auto _r_x1 = subdiv_6_3p1_1p1(x1);
		auto _r_x2 = subdiv_6_3p1_1p1(x2);
		auto _r_x3 = subdiv_6_3p1_1p1(x3);
		return {
			P3TetDegCGV{*this, 34,
				_r_m0[0], 
				_r_x0[0], 
				_r_x1[0], 
				_r_x2[0], 
				_r_x3[0]
			},
			P3TetDegCGV{*this, 35,
				_r_m0[1], 
				_r_x0[1], 
				_r_x1[1], 
				_r_x2[1], 
				_r_x3[1]
			},
			P3TetDegCGV{*this, 36,
				_r_m0[2], 
				_r_x0[2], 
				_r_x1[2], 
				_r_x2[2], 
				_r_x3[2]
			},
			P3TetDegCGV{*this, 37,
				_r_m0[3], 
				_r_x0[3], 
				_r_x1[3], 
				_r_x2[3], 
				_r_x3[3]
			},
			P3TetDegCGV{*this, 38,
				_r_m0[4], 
				_r_x0[4], 
				_r_x1[4], 
				_r_x2[4], 
				_r_x3[4]
			},
			P3TetDegCGV{*this, 39,
				_r_m0[5], 
				_r_x0[5], 
				_r_x1[5], 
				_r_x2[5], 
				_r_x3[5]
			},
			P3TetDegCGV{*this, 40,
				_r_m0[6], 
				_r_x0[6], 
				_r_x1[6], 
				_r_x2[6], 
				_r_x3[6]
			},
			P3TetDegCGV{*this, 41,
				_r_m0[7], 
				_r_x0[7], 
				_r_x1[7], 
				_r_x2[7], 
				_r_x3[7]
			},
		};
	}
	std::ostream &operator<<(std::ostream &out, const P3TetDegCGV &s) {
		for (unsigned i=0; i<s.numVertices; ++i) {
			out << s.getVertex(i) << ' ';
		}
		return out;
	}
}
