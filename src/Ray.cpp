#include "Ray.hpp"
#include "Line.hpp"

// Class Constructor
Ray::Ray(vec3d _origin, vec3d _direction) {
    origin = _origin;

    if (_direction.length() != 1)
        direction = _direction.normalize();
    else
        direction = _direction;
}

Ray::Ray() {}

// Class Destructor
Ray::~Ray() {}

vec3d Ray::getOrigin() {
    return origin;
}

vec3d Ray::getDirection() {
    return direction;
}

Line Ray::asLine(float length) {
    return Line(origin, origin + direction * length);
}