#include "distributions.h"


/* All density functions are calculated on the log scale.
 *
 * Invalid distribution parameters result in NaN, while
 * an out of range values to be evaluated result in -Inf.
 */
namespace dist {

double normal_pdf(double x, double mean, double sd) {
    if (sd <= 0) {
        return NaN;
    }

    auto sigma_sq = pow(sd, 2);
    return (
        (-0.5 * log(2 * M_PI * sigma_sq)) +
        ((-1 * pow(x - mean, 2)) / (2 * sigma_sq))
    );
}

double gamma_pdf(double x, double alpha, double beta) {
    if (x <= 0) {
        return NEG_INF;
    }

    if (alpha < 0 || beta < 0) {
        return NaN;
    }

    return (
        (alpha * log(beta)) - lgamma(alpha) +
        ((alpha - 1) * log(x)) - (beta * x)
    );
}

double poisson_pdf(double x, double lambda) {
    // source: http://www.masaers.com/2013/10/08/Implementing-Poisson-pmf.html

    if (x <= 0) {
        return NEG_INF;
    }

    return x * log(lambda) - lgamma(x + 1.0) - lambda;
}


double __poisson_gamma_s(double lambda,
                         const std::vector<double>& data,
                         double alpha,
                         double beta) {
    auto prior_density = dist::gamma_pdf(lambda, alpha, beta);
    std::vector<double> log_likelihoods(data.size());
    std::transform(data.begin(), data.end(),
                   log_likelihoods.begin(),
                   [&](double x) {
                       return dist::poisson_pdf(x, lambda);
                   });
    return prior_density + std::accumulate(log_likelihoods.begin(),
                                           log_likelihoods.end(),
                                           0.);
}

double __poisson_gamma_p(double lambda,
                         const std::vector<double>& data,
                         double alpha,
                         double beta) {
    auto prior_density = dist::gamma_pdf(lambda, alpha, beta);
    double log_likelihood = 0;

    #pragma omp parallel for reduction(+:log_likelihood)
    for (std::size_t i = 0; i < data.size(); ++i) {
        log_likelihood += dist::poisson_pdf(data[i], lambda);
    }

    return prior_density + log_likelihood;
}

double poisson_gamma(double lambda,
                     const std::vector<double>& data,
                     double alpha,
                     double beta,
                     bool parallel) {
    if (data.empty()) {
        throw std::invalid_argument("`data` must contain at least one value");
    }

    if (parallel) {
        return __poisson_gamma_p(lambda, data, alpha, beta);
    }
    return __poisson_gamma_s(lambda, data, alpha, beta);
}

} // namespace dist
