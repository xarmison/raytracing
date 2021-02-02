#ifndef VEC3D_H
#define VEC3D_H

#include <iostream>
#include <math.h>

using namespace std;

class vec3d {
    
    private:
        float x, y, z;

    public:

        vec3d(float _x, float _y, float _z);
        vec3d();
        ~vec3d();

        void setX(float _x);
        void setY(float _y);
        void setZ(float _z);

        vec3d operator = (const vec3d &vec);
        vec3d operator + (const vec3d vec);
        vec3d operator - (const vec3d vec);

        // Adiocina a inversão dos fatores na multiplicação de vetor por escalar 
        friend vec3d operator * (const vec3d &vec, float scalar);
        friend vec3d operator * (float scalar, const vec3d &vec);

        vec3d operator * (const vec3d vec);
        vec3d operator / (const vec3d vec);

        float length();

        vec3d normalize();

        void translate(vec3d vec);

        vec3d rotateYP(float yaw, float pitch);

        float distance(vec3d vec);

        float dot(vec3d vec);

        // linear interpolation
        vec3d lerp(vec3d a, vec3d b, float t);

        friend ostream &operator << (ostream &os, vec3d vec);

};

#endif // VEC3D_H