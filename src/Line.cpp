#include "Line.hpp"
#include "Ray.hpp"

// Class Constructor
Line::Line() {}

// Class Destructor
Line::~Line() {}

Line::Line(vec3d _a, vec3d _b) {
    a = _a;
    b = _b;
}

Ray Line::asRay() {
    return Ray(a, (b - a).normalize());
}