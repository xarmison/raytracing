#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;
using std::fabs;

class vec3 {
    public:
        double vec[3];
    public:
        vec3() : vec{0, 0, 0} {};
        vec3(double x, double y, double z) : vec{x, y, z} {};

        double x() const { 
            return vec[0]; 
        }
        
        double y() const { 
            return vec[1]; 
        }
        
        double z() const { 
            return vec[2]; 
        }

        double operator [] (int i) const { 
            return vec[i]; 
        }

        double& operator [] (int i) { 
            return vec[i]; 
        }

        vec3 operator - () const { 
            return vec3(-vec[0], -vec[1], -vec[2]);
        }

        vec3& operator += (const vec3 &v) {
            vec[0] += v.vec[0];
            vec[1] += v.vec[1];
            vec[2] += v.vec[2];

            return *this;
        }

        vec3& operator *= (const double t) {
            vec[0] *= t;
            vec[1] *= t;
            vec[2] *= t;

            return *this;
        }

        vec3& operator /= (const double t) {
            return *this *= 1 / t;
        }

        double length_squared() const {
            return vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
        }

        double length() const {
            return sqrt(length_squared());
        }

        // Return true if the vector is close to zero in all dimensions
        bool near_zero() const {
            const auto s = 1e-8;

            return (fabs(vec[0]) < s) and (fabs(vec[1]) < s) and (fabs(vec[2]) < s);
        }

        inline static vec3 random() {
            return vec3(
                random_double(), 
                random_double(), 
                random_double()
            );
        }

        inline static vec3 random(double min, double max) {
            return vec3(
                random_double(min, max), 
                random_double(min, max), 
                random_double(min, max)
            );
        }
};

// Type aliases for vec3
using point3 = vec3; // 3D Point
using color = vec3;  // RGB Color

// vec3 Utility Functions
inline std::ostream& operator << (std::ostream &out, const vec3 &v) {
    return out << "(" << v.vec[0] << " " << v.vec[1] << " " << v.vec[2] << ")";
}

inline vec3 operator + (const vec3 &u, const vec3 v) {
    return vec3(
        u.vec[0] + v.vec[0],
        u.vec[1] + v.vec[1],
        u.vec[2] + v.vec[2]
    );
}

inline vec3 operator - (const vec3 &u, const vec3 v) {
    return vec3(
        u.vec[0] - v.vec[0],
        u.vec[1] - v.vec[1],
        u.vec[2] - v.vec[2]
    );
}

inline vec3 operator * (const vec3 &u, const vec3 v) {
    return vec3(
        u.vec[0] * v.vec[0],
        u.vec[1] * v.vec[1],
        u.vec[2] * v.vec[2]
    );
}

inline vec3 operator * (double t, const vec3 &v) {
    return vec3(
        t * v.vec[0],
        t * v.vec[1],
        t * v.vec[2]
    );
}

inline vec3 operator * (const vec3 &v, double t) {
    return t * v;
}

inline vec3 operator / (vec3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const vec3 &u, const vec3& v) {
    return u.vec[0] * v.vec[0] + u.vec[1] * v.vec[1] + u.vec[2] * v.vec[2];
} 

inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(
        u.vec[1] * v.vec[2] - u.vec[2] * v.vec[1],
        u.vec[2] * v.vec[0] - u.vec[0] * v.vec[2],
        u.vec[0] * v.vec[1] - u.vec[1] * v.vec[0]
    );
}

inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

inline vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1, 1);
        
        if (p.length_squared() >= 1) 
            continue;

        return p;
    }
}

inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

inline vec3 random_in_hemisphere(const vec3 &normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();

    // In the same hemisphere as the normal
    if (dot(in_unit_sphere, normal) > 0.0)
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

inline vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - 2 * dot(v, n) * n;
}

#endif // VEC3_H