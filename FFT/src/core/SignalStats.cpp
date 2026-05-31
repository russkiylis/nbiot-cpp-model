#include "SignalStats.h"

#include <algorithm>

double MaxAbs(const std::vector<Complex>& values) {
    double result = 0.0;
    for (const auto& value : values) {
        result = std::max(result, value.abs());
    }
    return result;
}
