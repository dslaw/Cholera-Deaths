#include "utils.h"


std::vector<double> read_counts(std::string filename) {
    std::ifstream input_file(filename);

    if (!input_file) {
        std::stringstream ss;
        ss << "File not found: " << filename << "\n";
        throw std::runtime_error(ss.str());
    }

    std::size_t value;
    std::vector<double> counts;

    while (input_file >> value) {
        counts.push_back(static_cast<double>(value));
    }

    return counts;
}
