#ifndef __ADAPT_H
#define __ADAPT_H

#include <cmath>
#include <limits>
#include <random>
#include <stdexcept>
#include <vector>

#include "utils.h"


namespace adaptive {

const double TARGET_RATE = .44;

double adapt(double nu, double acceptance_rate, std::size_t j);
std::vector<double> sample(const std::vector<double>& data,
                           double init,
                           std::size_t n_samples,
                           posterior p,
                           double nu,
                           std::mt19937& generator,
                           std::size_t periodicity);

} // namespace adaptive

#endif
