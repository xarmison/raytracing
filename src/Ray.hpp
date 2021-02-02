#ifndef RAY_H
#define RAY_H

#include "Vec3D.hpp"

class Line;

class Ray {
    private:
        vec3d origin, direction;

    public:
        Ray(vec3d _origin, vec3d _direction);
        Ray();
        ~Ray();

        Line asLine(float length);

        vec3d getOrigin();

        vec3d getDirection();
    
};

#endif // RAY_H