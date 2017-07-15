#include "prefetching.h"


namespace prefetch {

// Draw random jump distances from the proposal distribution (normal).
std::vector<double> draw_jumps(double nu, std::size_t n, std::mt19937& generator) {
    std::vector<double> jump_distances(n);
    std::normal_distribution<double> jump_distribution(0, nu);

    std::generate(jump_distances.begin(),
                  jump_distances.end(),
                  [&](void) {
                      return jump_distribution(generator);
                  });

    return jump_distances;
}

// Create the tree of proposal values.
//
// The proposal tree is represented as a binary heap.
std::vector<double> proposal_tree(double theta, const std::vector<double>& distances) {
    const auto n = distances.size();
    std::vector<double> proposals(n);
    std::size_t i = 0;
    auto current = theta;

    while (i < n) {
        // Set proposal for current node.
        auto proposal = current + distances[i];
        proposals[i] = proposal;

        auto left_idx = (2 * i) + 1;
        auto right_idx = (2 * i) + 2;

        // Set left child with current value.
        if (left_idx < n) {
            proposals[left_idx] = current;
        }

        // Set right child with proposal value.
        if (right_idx < n) {
            proposals[right_idx] = proposal;
        }

        current = proposals[++i];
    }

    return proposals;
}

// Calculate proposal density for each node in the tree, in parallel.
std::vector<double> evaluate_proposals(const std::vector<double>& proposals,
                                       const std::vector<double>& data,
                                       posterior p) {
    std::vector<double> densities(proposals.size());

    #pragma omp parallel for
    for (std::size_t i = 0; i < proposals.size(); ++i) {
        densities[i] = p(proposals[i], data);
    }

    return densities;
}

// Draw MCMC samples via prefetching.
std::vector<double> draw(double x_t, double nu, std::size_t n,
                         std::mt19937& generator,
                         const std::vector<double>& data,
                         posterior p) {
    std::vector<double> draws;
    std::uniform_real_distribution<double> runiform(0, 1);

    auto distances = draw_jumps(nu, n, generator);
    auto proposals = proposal_tree(x_t, distances);
    auto proposal_densities = evaluate_proposals(proposals, data, p);

    std::size_t i = 0;
    auto current = x_t;
    auto current_density = p(x_t, data);

    while (i < n) {
        auto proposal_density = proposal_densities[i];
        auto r = proposal_density - current_density;
        auto u = log(runiform(generator));

        if (std::isfinite(r) && (u <= r)) {
            // Accept the proposal, updating the current value, and
            // go right.
            draws.push_back(proposals[i]);
            current = proposals[i];
            current_density = proposal_density;
            i = (2 * i) + 2;
        } else {
            // Reject the proposal and go left.
            draws.push_back(current);
            i = (2 * i) + 1;
        }
    }

    return draws;
}

} // namespace prefetch