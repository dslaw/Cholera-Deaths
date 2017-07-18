#ifndef __UTILS_H
#define __UTILS_H

#include <fstream>
#include <functional>
#include <sstream>
#include <string>
#include <vector>


typedef std::function<double(double, const std::vector<double>&)> posterior;
std::vector<double> read_counts(std::string filename);

#endif
