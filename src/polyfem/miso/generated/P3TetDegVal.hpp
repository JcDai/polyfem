#pragma once
#include "core/RealVector.hpp"
#include "core/SubdivHistory.hpp"

namespace miso {
	class P3TetDegVal {
		public:
		static constexpr unsigned dimension = 3;
		static constexpr unsigned numVertices = 4;
		static constexpr unsigned numSamples = 11;
		static constexpr unsigned numConstraints = 1;
		static constexpr bool hasObjective = false;
		unsigned scheme = 0;
		unsigned depth = 0;
		std::shared_ptr<const SubdivHistory> history;
		unsigned degenerateState = 0;
		unsigned alternation = 0;

		P3TetDegVal(
			const RealVector<20> &px,
			const RealVector<20> &py,
			const RealVector<20> &pz
		);

		P3TetDegVal() = default;

		unsigned schemeSize() const {
			auto s = (scheme < schemes.size()) ? scheme : 0;
			return schemes.at(s);
		}

		const Real width() const {
			return std::max({
				x0.inclusion().width(),
				x1.inclusion().width(),
				x2.inclusion().width()
			});
		}

		RealVector<dimension> getVertex(unsigned i) const {
			return {
				x0[i],
				x1[i],
				x2[i]
			};
		}

		static constexpr std::array<unsigned, 5> schemes = {8, 4, 4, 4, 4};

		static constexpr unsigned numDegenerateStates = 9;
		static unsigned degenerateStateFromVertices(unsigned vertexMask, unsigned order = 1);
		static unsigned degenerateStateVertexMask(unsigned s);
		static unsigned degenerateStateOrder(unsigned s);
		static unsigned degenerateTransition(unsigned s, unsigned q);

		static unsigned alternateScheme(unsigned s);
		unsigned effectiveScheme(unsigned s) const;

		RealInterval inclusion(unsigned i) const;
		RealVector<numSamples> sample(unsigned i) const;
		template<typename F> void split(unsigned scheme, F &&f) const {
			scheme = effectiveScheme(scheme);
			if (scheme == 4) { for (auto &&_c : split_impl<4>()) f(std::move(_c)); }
			else if (scheme == 3) { for (auto &&_c : split_impl<3>()) f(std::move(_c)); }
			else if (scheme == 2) { for (auto &&_c : split_impl<2>()) f(std::move(_c)); }
			else if (scheme == 1) { for (auto &&_c : split_impl<1>()) f(std::move(_c)); }
			else { for (auto &&_c : split_impl<0>()) f(std::move(_c)); }
		}
		void inherit(const P3TetDegVal &parent, unsigned q);
		friend std::ostream &operator<<(std::ostream &out, const P3TetDegVal &s);

		private:

		RealVector<84> m0;
		RealVector<4> x0;
		RealVector<4> x1;
		RealVector<4> x2;

		P3TetDegVal(const P3TetDegVal &parent, unsigned q,
			const RealVector<84> &m0,
			const RealVector<4> &x0,
			const RealVector<4> &x1,
			const RealVector<4> &x2
		);

		static RealVector<84> CL_m0(const RealVector<20> &px, const RealVector<20> &py, const RealVector<20> &pz);
		static RealVector<84> LB_3p6(const RealVector<84> &_l);
		static std::array<RealVector<4>, 8> subdiv_0_3p1(const RealVector<4> &_b);
		static std::array<RealVector<4>, 4> subdiv_1_3p1(const RealVector<4> &_b);
		static std::array<RealVector<4>, 4> subdiv_2_3p1(const RealVector<4> &_b);
		static std::array<RealVector<4>, 4> subdiv_3_3p1(const RealVector<4> &_b);
		static std::array<RealVector<4>, 4> subdiv_4_3p1(const RealVector<4> &_b);
		static std::array<RealVector<84>, 8> subdiv_0_3p6(const RealVector<84> &_b);
		static std::array<RealVector<84>, 4> subdiv_1_3p6(const RealVector<84> &_b);
		static std::array<RealVector<84>, 4> subdiv_2_3p6(const RealVector<84> &_b);
		static std::array<RealVector<84>, 4> subdiv_3_3p6(const RealVector<84> &_b);
		static std::array<RealVector<84>, 4> subdiv_4_3p6(const RealVector<84> &_b);
		static RealVector<11> SQ_m0(const RealVector<84> &_b);
		RealVector<84> degenerateZeroed_m0() const;
		RealVector<numSamples> sampleLanes_m0() const;
		double sampleError_m0() const;
		template<unsigned SI=0> std::array<P3TetDegVal, schemes[SI]> split_impl() const;
	};

	template<> std::array<P3TetDegVal, P3TetDegVal::schemes[0]> P3TetDegVal::split_impl<0>() const;
	template<> std::array<P3TetDegVal, P3TetDegVal::schemes[1]> P3TetDegVal::split_impl<1>() const;
	template<> std::array<P3TetDegVal, P3TetDegVal::schemes[2]> P3TetDegVal::split_impl<2>() const;
	template<> std::array<P3TetDegVal, P3TetDegVal::schemes[3]> P3TetDegVal::split_impl<3>() const;
	template<> std::array<P3TetDegVal, P3TetDegVal::schemes[4]> P3TetDegVal::split_impl<4>() const;
}
