#include <algorithm>
#include <iostream>
#include <omp.h>
#include <random>
#include <string>
#include <vector>

#include "adapt.h"
#include "distributions.h"
#include "generic.h"
#include "prefetching.h"
#include "utils.h"


void print_draws(const std::vector<double>& draws);


static const double alpha = 1,
                    beta = 1,
                    nu = 2.5,
                    init = 0.01;
static const std::size_t n_samples = 10000,
                         periodicity = 500,
                         OUTPUT_PRECISION = 10;
static std::size_t tree_nodes;


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cout << "Invalid number of arguments" << std::endl;
        return 1;
    }

    auto sampler = static_cast<std::string>(argv[1]);
    std::string sampler_lower(sampler);
    auto filename = static_cast<std::string>(argv[2]);

    // Assumes ASCII input, but all valid options are ASCII.
    std::transform(sampler_lower.begin(),
                   sampler_lower.end(),
                   sampler_lower.begin(),
                   ::tolower);

    std::mt19937 generator(13);
    auto posterior = [&](double lambda, const std::vector<double>& data) {
        return dist::poisson_gamma(lambda, data, alpha, beta, false);
    };
    auto parallel_posterior = [&](double lambda, const std::vector<double>& data) {
        return dist::poisson_gamma(lambda, data, alpha, beta, true);
    };

    if (sampler_lower == "adaptive") {
        auto data = read_counts(filename);
        auto draws = adaptive::sample(data, init, n_samples,
                                      posterior,
                                      nu, generator, periodicity);
        print_draws(draws);
    } else if (sampler_lower == "prefetching") {
        tree_nodes = omp_get_num_threads();
        auto data = read_counts(filename);
        auto draws = prefetch::sample(data, init, n_samples,
                                      posterior,
                                      nu, generator, tree_nodes);
        print_draws(draws);
    } else if (sampler_lower == "parallel") {
        auto data = read_counts(filename);
        auto draws = generic::sample(data, init, n_samples,
                                     parallel_posterior,
                                     nu, generator);
        print_draws(draws);
    } else {
        std::cout << "`" << sampler << "`"
                  << " is not a recgonized sampler"
                  << std::endl;
        return 1;
    }

}


void print_draws(const std::vector<double>& draws) {
    std::cout.precision(OUTPUT_PRECISION);
    for (auto& draw: draws) {
        std::cout << draw << std::endl;
    }
}
