#ifndef __PREFETCHING_H
#define __PREFETCHING_H

#include <algorithm>
#include <cmath>
#include <omp.h>
#include <random>
#include <stdexcept>
#include <utility>
#include <vector>

#include "utils.h"


namespace prefetch {

std::vector<double> draw_jumps(double nu, std::size_t n, std::mt19937& generator);
std::vector<double> proposal_tree(double theta, const std::vector<double>& distances);
std::vector<double> evaluate_proposals(const std::vector<double>& proposals,
                                       const std::vector<double>& data,
                                       posterior p);
std::vector<double> draw(double x_t, double nu, std::size_t n,
                         std::mt19937& generator,
                         const std::vector<double>& data,
                         posterior p);
std::vector<double> sample(const std::vector<double>& data,
                           double init,
                           std::size_t n_samples,
                           posterior p,
                           double nu,
                           std::mt19937& generator,
                           std::size_t n_nodes);

} // namespace prefetch

#endif
