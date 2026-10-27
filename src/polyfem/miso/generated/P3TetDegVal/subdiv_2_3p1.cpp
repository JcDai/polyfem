#include "../P3TetDegVal.hpp"
namespace miso {
	std::array<RealVector<4>, 4> P3TetDegVal::subdiv_2_3p1(const RealVector<4> &_b) {
		#pragma region Temporaries
		const Real _t0 = (0.5)*_b[0];
		const Real _t1 = (0.5)*_b[1];
		const Real _t2 = _t0 + _t1;
		const Real _t3 = (0.5)*_b[2];
		const Real _t4 = _t0 + _t3;
		const Real _t5 = _t1 + _t3;
		#pragma endregion
		#pragma region Expressions
		return {{
			{
				_b[0],
				_t2,
				_t4,
				_b[3],
			},
			{
				_t4,
				_t5,
				_b[2],
				_b[3],
			},
			{
				_t2,
				_b[1],
				_t5,
				_b[3],
			},
			{
				_t5,
				_t4,
				_t2,
				_b[3],
			},
		}};
		#pragma endregion
	}
}
