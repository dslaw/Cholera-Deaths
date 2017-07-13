#include "utils.h"


/* Read counts dataset.
 *
 * Throws `runtime_error` if the input file is not found.
 * Throws `domain_error` if any of the counts are negative.
 */
std::vector<std::size_t> read_counts(std::string filename) {
    std::ifstream input_file(filename);

    if (!input_file) {
        std::stringstream ss;
        ss << "File not found: " << filename << "\n";
        throw std::runtime_error(ss.str());
    }

    float value;
    std::vector<std::size_t> counts;

    while (input_file >> value) {
        if (value < 0) {
            std::stringstream ss;
            ss << "Invalid value: " << value << "\n";
            throw std::domain_error(ss.str());
        }
        counts.push_back((std::size_t) value);
    }

    return counts;
}
