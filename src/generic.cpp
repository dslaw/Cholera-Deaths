#include "generic.h"


namespace generic {

std::vector<double> sample(const std::vector<double>& data,
                           double init,
                           std::size_t n_samples,
                           posterior p,
                           double nu,
                           std::mt19937& generator) {
    if (n_samples < 1) {
        throw std::invalid_argument("`n_samples` must be positive");
    }

    std::normal_distribution<double> rnormal(0, nu);
    std::uniform_real_distribution<double> runiform(0, 1);

    std::vector<double> draws(n_samples);
    draws[0] = init;

    for (std::size_t t = 1; t < n_samples; ++t) {
        auto current = draws[t - 1];
        auto proposal = current + rnormal(generator);
        auto r = p(proposal, data) - p(current, data);
        auto u = log(runiform(generator));

        std::isfinite(r) && (u <= r)
            ? draws[t] = proposal       // Accept.
            : draws[t] = current;       // Reject.
    }

    return draws;
}

} // namespace generic
