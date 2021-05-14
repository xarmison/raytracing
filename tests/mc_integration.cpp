#include "../include/utility.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

inline double pdf(const vec3& p) {
    return 1 / (4 * pi);
}

int main() {
    int N = 1000000;
    double sum = 0.0;

    // Approximating the integral int cos^2 (theta)
    for (int i = 0; i < N; i++) {
        vec3 d = random_unit_vector();
        auto cosine_squared = d.z() * d.z();
        sum += cosine_squared / pdf(d);
    }

    std::cout << std::fixed << std::setprecision(12);
    std::cout << "Int = " << sum / N << '\n';

    return 0;
}