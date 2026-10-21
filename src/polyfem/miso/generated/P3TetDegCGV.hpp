#pragma once
#include "core/RealVector.hpp"
#include "core/SubdivHistory.hpp"

namespace miso {
	class P3TetDegCGV {
		public:
		static constexpr unsigned dimension = 4;
		static constexpr unsigned numVertices = 8;
		static constexpr unsigned numSamples = 22;
		static constexpr unsigned numConstraints = 1;
		static constexpr bool hasObjective = true;
		unsigned scheme = 0;
		unsigned depth = 0;
		std::shared_ptr<const SubdivHistory> history;
		unsigned degenerateState = 0;
		unsigned alternation = 0;

		P3TetDegCGV(
			const RealVector<20> &p0x,
			const RealVector<20> &p0y,
			const RealVector<20> &p0z,
			const RealVector<20> &p1x,
			const RealVector<20> &p1y,
			const RealVector<20> &p1z
		);

		P3TetDegCGV() = default;

		unsigned schemeSize() const {
			auto s = (scheme < schemes.size()) ? scheme : 0;
			return schemes.at(s);
		}

		const Real width() const {
			return std::max({
				x0.inclusion().width(),
				x1.inclusion().width(),
				x2.inclusion().width(),
				x3.inclusion().width()
			});
		}

		RealVector<dimension> getVertex(unsigned i) const {
			return {
				x0[i],
				x1[i],
				x2[i],
				x3[i]
			};
		}

		static constexpr std::array<unsigned, 7> schemes = {16, 2, 4, 4, 4, 4, 8};

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
			if (scheme == 6) { for (auto &&_c : split_impl<6>()) f(std::move(_c)); }
			else if (scheme == 5) { for (auto &&_c : split_impl<5>()) f(std::move(_c)); }
			else if (scheme == 4) { for (auto &&_c : split_impl<4>()) f(std::move(_c)); }
			else if (scheme == 3) { for (auto &&_c : split_impl<3>()) f(std::move(_c)); }
			else if (scheme == 2) { for (auto &&_c : split_impl<2>()) f(std::move(_c)); }
			else if (scheme == 1) { for (auto &&_c : split_impl<1>()) f(std::move(_c)); }
			else { for (auto &&_c : split_impl<0>()) f(std::move(_c)); }
		}
		void inherit(const P3TetDegCGV &parent, unsigned q);
		friend std::ostream &operator<<(std::ostream &out, const P3TetDegCGV &s);

		private:

		RealVector<336> m0;
		RealVector<8> x0;
		RealVector<8> x1;
		RealVector<8> x2;
		RealVector<8> x3;

		P3TetDegCGV(const P3TetDegCGV &parent, unsigned q,
			const RealVector<336> &m0,
			const RealVector<8> &x0,
			const RealVector<8> &x1,
			const RealVector<8> &x2,
			const RealVector<8> &x3
		);

		static RealVector<336> CL_m0(const RealVector<20> &p0x, const RealVector<20> &p0y, const RealVector<20> &p0z, const RealVector<20> &p1x, const RealVector<20> &p1y, const RealVector<20> &p1z);
		static RealVector<336> LB_3p6_1p3(const RealVector<336> &_l);
		static std::array<RealVector<8>, 16> subdiv_0_3p1_1p1(const RealVector<8> &_b);
		static std::array<RealVector<8>, 2> subdiv_1_3p1_1p1(const RealVector<8> &_b);
		static std::array<RealVector<8>, 4> subdiv_2_3p1_1p1(const RealVector<8> &_b);
		static std::array<RealVector<8>, 4> subdiv_3_3p1_1p1(const RealVector<8> &_b);
		static std::array<RealVector<8>, 4> subdiv_4_3p1_1p1(const RealVector<8> &_b);
		static std::array<RealVector<8>, 4> subdiv_5_3p1_1p1(const RealVector<8> &_b);
		static std::array<RealVector<8>, 8> subdiv_6_3p1_1p1(const RealVector<8> &_b);
		static std::array<RealVector<336>, 16> subdiv_0_3p6_1p3(const RealVector<336> &_b);
		static std::array<RealVector<336>, 2> subdiv_1_3p6_1p3(const RealVector<336> &_b);
		static std::array<RealVector<336>, 4> subdiv_2_3p6_1p3(const RealVector<336> &_b);
		static std::array<RealVector<336>, 4> subdiv_3_3p6_1p3(const RealVector<336> &_b);
		static std::array<RealVector<336>, 4> subdiv_4_3p6_1p3(const RealVector<336> &_b);
		static std::array<RealVector<336>, 4> subdiv_5_3p6_1p3(const RealVector<336> &_b);
		static std::array<RealVector<336>, 8> subdiv_6_3p6_1p3(const RealVector<336> &_b);
		static RealVector<22> SQ_m0(const RealVector<336> &_b);
		RealVector<336> degenerateZeroed_m0() const;
		RealVector<numSamples> sampleLanes_m0() const;
		double sampleError_m0() const;
		template<unsigned SI=0> std::array<P3TetDegCGV, schemes[SI]> split_impl() const;
	};

	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[0]> P3TetDegCGV::split_impl<0>() const;
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[1]> P3TetDegCGV::split_impl<1>() const;
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[2]> P3TetDegCGV::split_impl<2>() const;
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[3]> P3TetDegCGV::split_impl<3>() const;
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[4]> P3TetDegCGV::split_impl<4>() const;
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[5]> P3TetDegCGV::split_impl<5>() const;
	template<> std::array<P3TetDegCGV, P3TetDegCGV::schemes[6]> P3TetDegCGV::split_impl<6>() const;
}
