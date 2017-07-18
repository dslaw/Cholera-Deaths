#ifndef __GENERIC_H
#define __GENERIC_H

#include <cmath>
#include <random>
#include <stdexcept>
#include <vector>

#include "utils.h"


// Generic MCMC sampler.
namespace generic {

std::vector<double> sample(const std::vector<double>& data,
                           double init,
                           std::size_t n_samples,
                           posterior p,
                           double nu,
                           std::mt19937& generator);

} // namespace generic

#endif
