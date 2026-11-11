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

		/// AMIPS energy minus its pointwise minimum (d/n)^power (the value at
		/// the rest pose, or any similarity), so the energy is 0 at rest
		/// instead of a large constant. Value only: gradient and Hessian are
		/// unchanged.
		bool subtract_rest_value = false;

		/// Adaptive elastic stiffness, the strategy of IPC's adaptive barrier
		/// stiffness applied to the elastic form's weight kappa (a multiplier
		/// on the configured material weight): at the start of each nonlinear
		/// solve, kappa = -(g_E . g_B) / |g_B|^2 (g_B the elastic gradient at
		/// kappa = 1, g_E the sum of the other forms' gradients), clamped to
		/// [1, max_scale] (1 if g_B = 0); after each accepted step, kappa is
		/// doubled (up to max_scale) if the smallest det F over the elastic
		/// form's quadrature points is below det_threshold and decreasing.
		struct AdaptiveElasticStiffness
		{
			bool enabled = false;
			double max_scale = 100;
			double det_threshold = 1e-9;
		} adaptive_elastic_stiffness;
	};

	/// The process-wide settings.
	Settings &settings();

	/// Set settings() from the "experiments" section (missing keys keep their defaults).
	void set_from_json(const json &args);
} // namespace polyfem::experiments
