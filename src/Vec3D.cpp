#include "Vec3D.hpp"

vec3d::vec3d(float _x, float _y, float _z) {
    x = _x;
    y = _y;
    z = _z;
}

vec3d::vec3d() {}

vec3d::~vec3d() {}

void vec3d::setX(float _x) {
    x = _x;
}

void vec3d::setY(float _y) {
    y = _y;
}

void vec3d::setZ(float _z) {
    z = _z;
}

vec3d vec3d::operator = (const vec3d &vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;

    return *this;
}

vec3d vec3d::operator + (const vec3d vec) {
    return vec3d(x + vec.x, y + vec.y, z + vec.z);
}

vec3d vec3d::operator - (const vec3d vec) {
    return vec3d(x - vec.x, y - vec.y, z - vec.z);
}

vec3d operator * (const vec3d &vec, float scalar) {
    return vec3d(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

// Adiocina a inversão dos fatores na multiplicação de vetor por escalar
vec3d operator * (float scalar, const vec3d &vec) {
    return vec * scalar;
}

vec3d vec3d::operator * (const vec3d vec) {
    return vec3d(x * vec.x, y * vec.y, z * vec.z);
}

vec3d vec3d::operator / (const vec3d vec) {
    return vec3d(x / vec.x, y / vec.y, z / vec.z);
}

float vec3d::length() {
    return (float)sqrt(x * x + y * y + z * z);
}

vec3d vec3d::normalize() {
    float len = length();

    return vec3d(x / len, y / len, z / len);
}

void vec3d::translate(vec3d vec) {
    x += vec.x;
    y += vec.y;
    z += vec.z;
}

vec3d vec3d::rotateYP(float yaw, float pitch) {
    double factor = 3.14159265359 / 180;

    // Convert to radians
    double yaw_rad = yaw * factor;
    double pitch_rad = pitch * factor;

    // Step one: Rotate around X axis (pitch)
    float _y = (float)(y * cos(pitch_rad) - z * sin(pitch_rad));
    float _z = (float)(y * sin(pitch_rad) + z * cos(pitch_rad));

    // Step two: Rotate around the Y axis (yaw)
    float _x = (float)(x * cos(yaw_rad) + _z * sin(yaw_rad));
    _z = (float)(-x * sin(yaw_rad) + _z * cos(yaw_rad));

    return vec3d(_x, _y, _z);
}

float vec3d::distance(vec3d vec) {
    return (float)sqrt(
        pow(x - vec.x, 2) +
        pow(y - vec.y, 2) +
        pow(z - vec.z, 2));
}

float vec3d::dot(vec3d vec) {
    return x + vec.x, y + vec.y, z + vec.z;
}

// linear interpolation
vec3d vec3d::lerp(vec3d a, vec3d b, float t) {
    return a + t * (b - a);
}

ostream &operator << (ostream &os, vec3d vec) {
    os << "[ " << vec.x << ", " << vec.y << ", " << vec.z << "]";

    return os;
}