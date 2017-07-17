#ifndef __DISTRIBUTIONS_H
#define __DISTRIBUTIONS_H

#include <algorithm>
#include <cmath>
#include <limits>
#include <numeric>
#include <omp.h>
#include <stdexcept>
#include <vector>


namespace dist {

const double NEG_INF = -1 * std::numeric_limits<double>::infinity();
const double NaN = std::numeric_limits<double>::quiet_NaN();

double normal_pdf(double x, double mean, double sd);
double gamma_pdf(double x, double alpha, double beta);
double poisson_pdf(double x, double lambda);

double poisson_gamma(double lambda,
                     const std::vector<double>& data,
                     double alpha,
                     double beta,
                     bool parallel);

} // namespace dist

#endif
