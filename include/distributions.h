#ifndef __DISTRIBUTIONS_H
#define __DISTRIBUTIONS_H

#include <cmath>
#include <limits>


namespace dist {

double normal_pdf(double x, double mean, double sd);
double gamma_pdf(double x, double alpha, double beta);
double poisson_pdf(double x, double lambda);

}

#endif
