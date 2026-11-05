#include "MatrixLagrangianForm.hpp"

#include <polyfem/utils/MatrixUtils.hpp>
#include <cmath>
#include <algorithm>
#include <polyfem/utils/Logger.hpp>

namespace polyfem::solver
{

	MatrixLagrangianForm::MatrixLagrangianForm(const StiffnessMatrix &A,
											   const Eigen::MatrixXd &b,
											   const StiffnessMatrix &A_proj,
											   const Eigen::MatrixXd &b_proj)
	{
		A_ = A;
		b_ = b;
		A_proj_ = A_proj;
		b_proj_ = b_proj;

		assert(A_.rows() == b_.rows());
		AtA = A_.transpose() * A_;
		Atb = A_.transpose() * b_;

		lagr_mults_.resize(A_.rows());
		lagr_mults_.setZero();
	}

	MatrixLagrangianForm::MatrixLagrangianForm(const std::vector<double> &times,
											   const std::vector<std::shared_ptr<MatrixLagrangianForm>> &frames)
		: times_(times), frames_(frames)
	{
		if (times.empty() || times.size() != frames.size())
			log_and_throw_error("Constraint time sequence must be nonempty with one frame per timestamp");
		for (size_t i = 0; i < times.size(); ++i)
			if (!std::isfinite(times[i]) || (i && times[i] <= times[i - 1]))
				log_and_throw_error("Constraint timestamps must be finite and strictly increasing");
		update_quantities(times.front(), Eigen::VectorXd());
	}

	void MatrixLagrangianForm::update_quantities(double t, const Eigen::VectorXd &)
	{
		if (times_.empty())
			return;
		int index = t <= times_.front() ? 0 : (t >= times_.back() ? int(times_.size()) - 1 : -1);
		for (size_t i = 0; i < times_.size(); ++i)
			if (std::abs(t - times_[i]) <= 1e-10 * std::max(1., std::abs(t)))
			{
				index = int(i);
				break;
			}
		if (index < 0)
			log_and_throw_error("No constraint frame matches simulation time {}", t);
		if (index == active_frame_)
			return;
		const auto &frame = *frames_[index];
		A_ = frame.A_;
		b_ = frame.b_;
		A_proj_ = frame.A_proj_;
		b_proj_ = frame.b_proj_;
		AtA = frame.AtA;
		Atb = frame.Atb;
		lagr_mults_ = Eigen::VectorXd::Zero(A_.rows());
		active_frame_ = index;
	}

	double MatrixLagrangianForm::value_unweighted(const Eigen::VectorXd &x) const
	{
		const Eigen::VectorXd res = A_ * x - b_;
		const double L_penalty = lagr_mults_.transpose() * res;
		const double A_penalty = res.squaredNorm() / 2;
		return L_weight() * L_penalty + A_weight() * A_penalty;
	}

	void MatrixLagrangianForm::first_derivative_unweighted(const Eigen::VectorXd &x, Eigen::VectorXd &gradv) const
	{
		gradv = L_weight() * A_.transpose() * lagr_mults_ + A_weight() * (AtA * x - Atb);
	}

	void MatrixLagrangianForm::second_derivative_unweighted(const Eigen::VectorXd &x, StiffnessMatrix &hessian) const
	{
		hessian = A_weight() * AtA;
	}

	void MatrixLagrangianForm::update_lagrangian(const Eigen::VectorXd &x, const double k_al)
	{
		k_al_ = k_al;
		lagr_mults_ += k_al * (A_ * x - b_);
	}

	double MatrixLagrangianForm::compute_error(const Eigen::VectorXd &x) const
	{
		const Eigen::VectorXd res = A_ * x - b_;
		return res.squaredNorm();
	}

} // namespace polyfem::solver
