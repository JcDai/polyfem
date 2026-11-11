#include "Experiments.hpp"

#include <polyfem/utils/Logger.hpp>

namespace polyfem::experiments
{
	Settings &settings()
	{
		static Settings s;
		return s;
	}

	void set_from_json(const json &args)
	{
		Settings s;
		if (args.contains("experiments") && args["experiments"].is_object())
		{
			const json &e = args["experiments"];
			s.robust_jacobian = e.value("robust_jacobian", s.robust_jacobian);
			s.softening_eps = e.value("softening_eps", s.softening_eps);
			s.subtract_rest_value = e.value("subtract_rest_value", s.subtract_rest_value);
			if (e.contains("adaptive_elastic_stiffness") && e["adaptive_elastic_stiffness"].is_object())
			{
				const json &a = e["adaptive_elastic_stiffness"];
				auto &t = s.adaptive_elastic_stiffness;
				t.enabled = a.value("enabled", t.enabled);
				t.max_scale = a.value("max_scale", t.max_scale);
				t.det_threshold = a.value("det_threshold", t.det_threshold);
			}
		}
		settings() = s;

		if (s.subtract_rest_value)
			logger().info("Experiments: AMIPS rest value subtracted");
		if (s.adaptive_elastic_stiffness.enabled)
			logger().info("Experiments: adaptive elastic stiffness, kappa in [1, {}], doubling below det F {}",
						  s.adaptive_elastic_stiffness.max_scale, s.adaptive_elastic_stiffness.det_threshold);

		if (s.robust_jacobian)
			logger().info("Experiments: robust det J in AMIPS");
		if (s.softening_eps > 0)
		{
			if (s.robust_jacobian)
				logger().info("Experiments: AMIPS softening, eps = {}", s.softening_eps);
			else
				logger().warn("Experiments: softening_eps is ignored without robust_jacobian");
		}
	}
} // namespace polyfem::experiments
