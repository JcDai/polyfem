#pragma once
#include "core/RealVector.hpp"
#include "core/SubdivHistory.hpp"

namespace miso {
	class P3TriDegVal {
		public:
		static constexpr unsigned dimension = 2;
		static constexpr unsigned numVertices = 3;
		static constexpr unsigned numSamples = 6;
		static constexpr unsigned numConstraints = 1;
		static constexpr bool hasObjective = false;
		unsigned scheme = 0;
		unsigned depth = 0;
		std::shared_ptr<const SubdivHistory> history;
		unsigned degenerateState = 0;

		P3TriDegVal(
			const RealVector<10> &px,
			const RealVector<10> &py
		);

		P3TriDegVal() = default;

		unsigned schemeSize() const {
			auto s = (scheme < schemes.size()) ? scheme : 0;
			return schemes.at(s);
		}

		const Real width() const {
			return std::max({
				x0.inclusion().width(),
				x1.inclusion().width()
			});
		}

		RealVector<dimension> getVertex(unsigned i) const {
			return {
				x0[i],
				x1[i]
			};
		}

		static constexpr std::array<unsigned, 1> schemes = {4};

		static constexpr unsigned numDegenerateStates = 4;
		static unsigned degenerateStateFromVertices(unsigned vertexMask, unsigned order = 1);
		static unsigned degenerateStateVertexMask(unsigned s);
		static unsigned degenerateStateOrder(unsigned s);
		static unsigned degenerateTransition(unsigned s, unsigned q);

		RealInterval inclusion(unsigned i) const;
		RealVector<numSamples> sample(unsigned i) const;
		template<typename F> void split(unsigned scheme, F &&f) const {
			for (auto &&_c : split_impl<0>()) f(std::move(_c));
		}
		void inherit(const P3TriDegVal &parent, unsigned q);
		friend std::ostream &operator<<(std::ostream &out, const P3TriDegVal &s);

		private:

		RealVector<15> m0;
		RealVector<3> x0;
		RealVector<3> x1;

		P3TriDegVal(const P3TriDegVal &parent, unsigned q,
			const RealVector<15> &m0,
			const RealVector<3> &x0,
			const RealVector<3> &x1
		);

		static RealVector<15> CL_m0(const RealVector<10> &px, const RealVector<10> &py);
		static RealVector<15> LB_2p4(const RealVector<15> &_l);
		static std::array<RealVector<3>, 4> subdiv_0_2p1(const RealVector<3> &_b);
		static std::array<RealVector<15>, 4> subdiv_0_2p4(const RealVector<15> &_b);
		static RealVector<6> SQ_m0(const RealVector<15> &_b);
		RealVector<15> degenerateZeroed_m0() const;
		RealVector<numSamples> sampleLanes_m0() const;
		double sampleError_m0() const;
		template<unsigned SI=0> std::array<P3TriDegVal, schemes[SI]> split_impl() const;
	};

	template<> std::array<P3TriDegVal, P3TriDegVal::schemes[0]> P3TriDegVal::split_impl<0>() const;
}
