#ifndef __DISTRIBUTIONS_H
#define __DISTRIBUTIONS_H

#include <cmath>
#include <limits>


namespace dist {

const double NEG_INF = -1 * std::numeric_limits<double>::infinity();
const double NaN = std::numeric_limits<double>::quiet_NaN();

double normal_pdf(double x, double mean, double sd);
double gamma_pdf(double x, double alpha, double beta);
double poisson_pdf(double x, double lambda);

}

#endif
