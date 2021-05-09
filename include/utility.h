#ifndef UTILITY_H
#define UTILITY_H

#include <cmath>
#include <random>
#include <limits>
#include <memory>

// Usings

using std::make_shared;
using std::shared_ptr;
using std::sqrt;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Common Headers

#include "ray.h"
#include "vec3.h"

// Random number generation

// Returns a random real number in [0, 1)
inline double random_double() {
    static std::uniform_real_distribution<double> distribuition(0.0, 1.0);
    static std::mt19937 generator;

    return distribuition(generator);
}

// Returns a random real number in [min,max)
inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

#endif  // UTILITY_H