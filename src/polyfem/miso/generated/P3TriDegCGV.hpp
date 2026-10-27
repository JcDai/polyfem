#pragma once
#include "core/RealVector.hpp"
#include "core/SubdivHistory.hpp"

namespace miso {
	class P3TriDegCGV {
		public:
		static constexpr unsigned dimension = 3;
		static constexpr unsigned numVertices = 6;
		static constexpr unsigned numSamples = 12;
		static constexpr unsigned numConstraints = 1;
		static constexpr bool hasObjective = true;
		unsigned scheme = 0;
		unsigned depth = 0;
		std::shared_ptr<const SubdivHistory> history;
		unsigned degenerateState = 0;
		unsigned alternation = 0;

		P3TriDegCGV(
			const RealVector<10> &p0x,
			const RealVector<10> &p0y,
			const RealVector<10> &p1x,
			const RealVector<10> &p1y
		);

		P3TriDegCGV() = default;

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

		static constexpr std::array<unsigned, 3> schemes = {8, 2, 4};

		static constexpr unsigned numDegenerateStates = 4;
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
			if (scheme == 2) { for (auto &&_c : split_impl<2>()) f(std::move(_c)); }
			else if (scheme == 1) { for (auto &&_c : split_impl<1>()) f(std::move(_c)); }
			else { for (auto &&_c : split_impl<0>()) f(std::move(_c)); }
		}
		void inherit(const P3TriDegCGV &parent, unsigned q);
		friend std::ostream &operator<<(std::ostream &out, const P3TriDegCGV &s);

		private:

		RealVector<45> m0;
		RealVector<6> x0;
		RealVector<6> x1;
		RealVector<6> x2;

		P3TriDegCGV(const P3TriDegCGV &parent, unsigned q,
			const RealVector<45> &m0,
			const RealVector<6> &x0,
			const RealVector<6> &x1,
			const RealVector<6> &x2
		);

		static RealVector<45> CL_m0(const RealVector<10> &p0x, const RealVector<10> &p0y, const RealVector<10> &p1x, const RealVector<10> &p1y);
		static RealVector<45> LB_2p4_1p2(const RealVector<45> &_l);
		static std::array<RealVector<6>, 8> subdiv_0_2p1_1p1(const RealVector<6> &_b);
		static std::array<RealVector<6>, 2> subdiv_1_2p1_1p1(const RealVector<6> &_b);
		static std::array<RealVector<6>, 4> subdiv_2_2p1_1p1(const RealVector<6> &_b);
		static std::array<RealVector<45>, 8> subdiv_0_2p4_1p2(const RealVector<45> &_b);
		static std::array<RealVector<45>, 2> subdiv_1_2p4_1p2(const RealVector<45> &_b);
		static std::array<RealVector<45>, 4> subdiv_2_2p4_1p2(const RealVector<45> &_b);
		static RealVector<12> SQ_m0(const RealVector<45> &_b);
		RealVector<45> degenerateZeroed_m0() const;
		RealVector<numSamples> sampleLanes_m0() const;
		double sampleError_m0() const;
		template<unsigned SI=0> std::array<P3TriDegCGV, schemes[SI]> split_impl() const;
	};

	template<> std::array<P3TriDegCGV, P3TriDegCGV::schemes[0]> P3TriDegCGV::split_impl<0>() const;
	template<> std::array<P3TriDegCGV, P3TriDegCGV::schemes[1]> P3TriDegCGV::split_impl<1>() const;
	template<> std::array<P3TriDegCGV, P3TriDegCGV::schemes[2]> P3TriDegCGV::split_impl<2>() const;
}
