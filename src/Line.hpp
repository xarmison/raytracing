#ifndef LINE_H
#define LINE_H

#include "Vec3D.hpp"

class Ray;

class Line {
    private:
        vec3d a, b;
    
    public:
        Line();
        ~Line();
        Line(vec3d _a, vec3d _b);

        Ray asRay();
    
};

#endif // LINE_H