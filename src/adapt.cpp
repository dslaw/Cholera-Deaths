#include "adapt.h"


namespace adaptive {

/// Update hyperparameter nu.
///
/// @param nu               Variance of the (normal) proposal distribution.
/// @param acceptance_rate  Current acceptance rate.
/// @param j                Current adaptation number (positive).
double adapt(double nu, double acceptance_rate, std::size_t j) {
    double s,
           i = static_cast<double>(j);

    if (acceptance_rate < TARGET_RATE) {
        s = -1;
    } else if (acceptance_rate > TARGET_RATE) {
        s = 1;
    } else {
        return nu;
    }

    return exp(log(nu) + (s / i));
}

/// Univariate MCMC with diminishing adaptation.
///
/// @param data         Dataset to calculate the log-likelihood over.
/// @param init         Initial value of the chain (i.e. \theta_{0})
/// @param n_samples    Number of samples to draw (positive).
/// @param p            Posterior function (log).
/// @param nu           Variance of the (normal) proposal distribution.
/// @param generator    PRNG.
/// @param periodicity  How frequently adaptation should be performed
///                     (positive).
std::vector<double> sample(const std::vector<double>& data,
                           double init,
                           std::size_t n_samples,
                           posterior p,
                           double nu,
                           std::mt19937& generator,
                           std::size_t periodicity) {
    if (n_samples < 1) {
        throw std::invalid_argument("`n_samples` must be positive");
    }

    if (periodicity < 1) {
        throw std::invalid_argument("`periodicity` must be positive");
    }

    const auto adaptive_cutoff = n_samples / 2;
    std::size_t accepted = 0;
    std::normal_distribution<double> stdnormal(0, 1);
    std::uniform_real_distribution<double> runiform(0, 1);

    std::vector<double> draws(n_samples);
    draws[0] = init;

    for (std::size_t t = 1; t < n_samples; ++t) {
        if (t > 0 && t < adaptive_cutoff && (t % periodicity) == 0) {
            auto j = t / periodicity;  // jth adaptation.
            nu = adapt(nu, static_cast<double>(accepted / t), j);
        }

        auto current = draws[t - 1];
        auto proposal = current + sqrt(nu) * stdnormal(generator);  // N(current, nu)
        auto r = p(proposal, data) - p(current, data);
        auto u = log(runiform(generator));

        if (std::isfinite(r) && (u <= r)) {
            ++accepted;
            draws[t] = proposal;
        } else {
            draws[t] = current;
        }
    }

    return draws;
}

} // namespace adaptive
