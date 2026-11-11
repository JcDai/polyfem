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
		}
		settings() = s;

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
