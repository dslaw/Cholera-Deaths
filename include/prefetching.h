#ifndef __PREFETCHING_H
#define __PREFETCHING_H

#include <algorithm>
#include <cmath>
#include <functional>
#include <omp.h>
#include <random>
#include <utility>
#include <vector>


namespace prefetch {

typedef std::function<double(double, const std::vector<double>&)> posterior;

std::vector<double> draw_jumps(double nu, std::size_t n, std::mt19937& generator);
std::vector<double> proposal_tree(double theta, const std::vector<double>& distances);
std::vector<double> evaluate_proposals(const std::vector<double>& proposals,
                                       const std::vector<double>& data,
                                       posterior p);
std::vector<double> draw(double x_t, double nu, std::size_t n,
                         std::mt19937& generator,
                         const std::vector<double>& data,
                         posterior p);

} // namespace prefetch

#endif
