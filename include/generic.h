#ifndef __GENERIC_H
#define __GENERIC_H

#include <cmath>
#include <functional>
#include <random>
#include <vector>


// Generic MCMC sampler.
namespace generic {

typedef std::function<double(double, const std::vector<double>&)> posterior;

std::vector<double> sample(const std::vector<double>& data,
                           double init,
                           std::size_t n_samples,
                           posterior p,
                           double nu,
                           std::mt19937& generator);

} // namespace generic

#endif
