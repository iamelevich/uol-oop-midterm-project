//
// Created by Ilya Amelevich on 31/12/2022.
//

#include "MathHelper.h"

double MathHelper::mean(std::vector<double> values) {
    // Sort values
    std::sort(values.begin(), values.end());

    // If no values - return 0.0
    if (values.empty()) {
        return 0.0;
    }
    // If prices size is even - return average of middle elements
    if (values.size() % 2 == 0) {
        int midIndex = (int) (values.size() / 2);
        return (values[midIndex] + values[midIndex - 1]) / 2;
    }
    // Return middle element
    return *(values.begin() + values.size() / 2);
}
