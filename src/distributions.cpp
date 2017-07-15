#include "distributions.h"


/* All density functions are calculated for the log.
 *
 * Invalid distribution parameters result in NaN, while
 * an out of range values to be evaluated result in -Inf.
 */
namespace dist {

const double NEG_INF = -1 * std::numeric_limits<double>::infinity();
const double NaN = std::numeric_limits<double>::quiet_NaN();


/* Probability distribution density functions. */
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

} // namespace dist
