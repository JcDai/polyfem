#include "EnergyTrace.hpp"

#include <polyfem/utils/Logger.hpp>
#include <polyfem/utils/Experiments.hpp>

#include <filesystem>
#include <map>
#include <memory>

namespace polyfem::solver::energy_trace
{
	const std::string &directory()
	{
		// Set once, the first time it is asked (after the input is read).
		static const std::string dir = [] {
			const std::string d = experiments::settings().energy_trace;
			if (d.empty())
				return std::string();
			std::filesystem::create_directories(d);
			logger().info("Energy trace: writing CSV files to {}", d);
			return d;
		}();
		return dir;
	}

	std::ofstream &csv(const std::string &name, const std::string &header)
	{
		static std::map<std::string, std::unique_ptr<std::ofstream>> streams;
		auto &stream = streams[name];
		if (!stream)
		{
			stream = std::make_unique<std::ofstream>(std::filesystem::path(directory()) / name, std::ios::trunc);
			stream->precision(17);
			*stream << header << '\n';
		}
		return *stream;
	}

	Counters &counters()
	{
		static Counters c;
		return c;
	}
} // namespace polyfem::solver::energy_trace
