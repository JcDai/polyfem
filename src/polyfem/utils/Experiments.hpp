#pragma once

#include <polyfem/Common.hpp>

namespace polyfem::experiments
{
	/// Experimental options from the "experiments" section of the input JSON.
	/// They are read once when a variational form is initialized and apply to
	/// the whole process. All of them are off by default.
	struct Settings
	{
		/// AMIPS takes det F from the robust (certified) evaluation the
		/// conservative inversion checks use, as the ratio of the reference
		/// dets of the deformed and rest maps, instead of from F in floating
		/// point. Only with a conservative inversion check.
		bool robust_jacobian = false;

		/// Softening of AMIPS: tr(F^T F) is replaced by
		/// S(T) = T + eps (1 - T / eps)^3 for T < eps (C2, S >= T, so the
		/// energy keeps its minimum). 0 = off. Robust path only, so it needs
		/// robust_jacobian.
		double softening_eps = 0;
	};

	/// The process-wide settings.
	Settings &settings();

	/// Set settings() from the "experiments" section (missing keys keep their defaults).
	void set_from_json(const json &args);
} // namespace polyfem::experiments
