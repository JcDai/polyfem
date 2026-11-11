#pragma once

#include <fstream>
#include <string>

namespace polyfem::solver::energy_trace
{
	/// Debug tracing of the energy during the nonlinear solve, for diagnosing
	/// line searches that stall against the conservative inversion check.
	///
	/// Off unless experiments/energy_trace names a directory; CSV files are
	/// then written there:
	///   iterations.csv   solve,state,form,value: every enabled form's weighted
	///                    energy after each accepted step, their total, the
	///                    gradient norms and the elastic form's weight (NLProblem)
	/// tools/plot_energy_terms.py plots them.

	/// Trace directory, or empty when tracing is off.
	const std::string &directory();

	inline bool enabled() { return !directory().empty(); }

	/// Stream appending to @p name in the trace directory. The file is
	/// truncated and @p header written the first time it is requested in
	/// this process.
	std::ofstream &csv(const std::string &name, const std::string &header);

	/// Counters that let rows of different files be joined.
	struct Counters
	{
		/// Nonlinear solves started (NLProblem::init), 0-based once started.
		int solve = -1;
		/// Accepted states in the current solve: 0 is the initial guess and k
		/// the state after the k-th step. A line search starting from state k
		/// is the one polysolve logs as "pre LS iter=k".
		int state = -1;
		/// Continuous Jacobian checks so far (all solves).
		int check = -1;
	};
	Counters &counters();
} // namespace polyfem::solver::energy_trace
