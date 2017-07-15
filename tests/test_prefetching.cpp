#include <algorithm>
#include <assert.h>
#include "prefetching.h"


double mock_posterior(double theta, const std::vector<double>& data);

void test_draw_jumps(void);
void test_proposal_tree(void);
void test_proposal_tree_sizes(void);
void test_evaluate_proposals(void);
void test_draw(void);


int main() {
    test_draw_jumps();
    test_proposal_tree();
    test_proposal_tree_sizes();
    test_evaluate_proposals();
    test_draw();
}


// Fixtures.
double mock_posterior(double theta, const std::vector<double>& data) {
    return theta + std::accumulate(data.begin(), data.end(), 0);
}


// Tests.
void test_draw_jumps(void) {
    // Smoke test.
    const double nu = 1;
    const std::size_t n = 10;
    std::mt19937 generator(13);

    auto jumps = prefetch::draw_jumps(nu, n, generator);
    assert (jumps.size() == n);
}

void test_proposal_tree(void) {
    const double theta = 1;
    const std::vector<double> jumps = {1, 2, 3, 4, 5};

    auto proposals = prefetch::proposal_tree(theta, jumps);
    // Cast to int for easier comparison.
    std::vector<int> out(proposals.size());
    std::transform(proposals.begin(), proposals.end(),
                   out.begin(),
                   [&](auto x) { return int(x); });

    // (current, proposal)
    //            (1, 2)
    //      (1, 3)      (2, 5)
    //  (1, 5)  (3, 8)
    assert (out[0] == 2);  // Root
    assert (out[1] == 3);  // Left child of [0]
    assert (out[2] == 5);  // Right child of [0]
    assert (out[3] == 5);  // Left child of [1]
    assert (out[4] == 8);  // Right child of [1]
}

void test_proposal_tree_sizes(void) {
    // Test binary heap assignment for different heap sizes.

    const double theta = 0;
    const std::vector<double> empty_vector(0);
    const std::vector<double> single(1);
    const std::vector<double> odd_sized(11);
    const std::vector<double> even_sized(10);

    auto empty_out = prefetch::proposal_tree(theta, empty_vector);
    auto single_out = prefetch::proposal_tree(theta, single);
    auto odd_out = prefetch::proposal_tree(theta, odd_sized);
    auto even_out = prefetch::proposal_tree(theta, even_sized);

    assert (empty_out.size() == empty_vector.size());
    assert (single_out.size() == single.size());
    assert (odd_out.size() == odd_sized.size());
    assert (even_out.size() == even_sized.size());
}

void test_evaluate_proposals(void) {
    const std::vector<double> counts = {1, 1, 2};
    const std::vector<double> proposals = {3, 4};
    const std::vector<double> expected = {7, 8};

    auto out = prefetch::evaluate_proposals(proposals,
                                            counts,
                                            mock_posterior);
    assert (out.size() == expected.size());
    auto all_eq = std::equal(expected.begin(),
                             expected.end(),
                             out.begin());
    assert (all_eq);
}

void test_draw(void) {
    // Smoke test.
    const double theta = 0;
    const double nu = 1;
    const std::size_t n_nodes = 7;
    std::mt19937 generator(13);
    const std::vector<double> counts = {1, 1, 2};

    auto out = prefetch::draw(theta, nu, n_nodes, generator,
                              counts, mock_posterior);
    assert (out.size() == 3);
}
