#include "../P3TetDegCGV.hpp"
namespace miso {
	std::array<RealVector<8>, 8> P3TetDegCGV::subdiv_6_3p1_1p1(const RealVector<8> &_b) {
		#pragma region Temporaries
		const Real _t0 = (0.5)*_b[0];
		const Real _t1 = (0.5)*_b[2];
		const Real _t2 = _t0 + _t1;
		const Real _t3 = (0.5)*_b[1];
		const Real _t4 = (0.5)*_b[3];
		const Real _t5 = _t3 + _t4;
		const Real _t6 = (0.5)*_b[4];
		const Real _t7 = _t0 + _t6;
		const Real _t8 = (0.5)*_b[5];
		const Real _t9 = _t3 + _t8;
		const Real _t10 = (0.5)*_b[6];
		const Real _t11 = _t0 + _t10;
		const Real _t12 = (0.5)*_b[7];
		const Real _t13 = _t12 + _t3;
		const Real _t14 = _t1 + _t10;
		const Real _t15 = _t12 + _t4;
		const Real _t16 = _t10 + _t6;
		const Real _t17 = _t12 + _t8;
		const Real _t18 = _t1 + _t6;
		const Real _t19 = _t4 + _t8;
		#pragma endregion
		#pragma region Expressions
		return {{
			{
				_b[0],
				_b[1],
				_t2,
				_t5,
				_t7,
				_t9,
				_t11,
				_t13,
			},
			{
				_t11,
				_t13,
				_t14,
				_t15,
				_t16,
				_t17,
				_b[6],
				_b[7],
			},
			{
				_t7,
				_t9,
				_t18,
				_t19,
				_b[4],
				_b[5],
				_t16,
				_t17,
			},
			{
				_t2,
				_t5,
				_b[2],
				_b[3],
				_t18,
				_t19,
				_t14,
				_t15,
			},
			{
				_t11,
				_t13,
				_t14,
				_t15,
				_t2,
				_t5,
				_t7,
				_t9,
			},
			{
				_t11,
				_t13,
				_t14,
				_t15,
				_t16,
				_t17,
				_t7,
				_t9,
			},
			{
				_t7,
				_t9,
				_t18,
				_t19,
				_t14,
				_t15,
				_t2,
				_t5,
			},
			{
				_t7,
				_t9,
				_t18,
				_t19,
				_t14,
				_t15,
				_t16,
				_t17,
			},
		}};
		#pragma endregion
	}
}
