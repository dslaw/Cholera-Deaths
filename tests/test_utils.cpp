#include <assert.h>
#include <algorithm>

#include "utils.h"


void test_read_counts_smoke(void);

int main() {
}


void test_read_counts_smoke(void) {
    std::size_t expected_len = 250;
    auto data = read_counts("data/cholera_counts.txt");

    assert( data.size() == expected_len );
    auto all_inbounds = std::all_of(data.begin(), data.end(),
                                    [](auto x) { return x >= 0; });
    assert( all_inbounds );
}
