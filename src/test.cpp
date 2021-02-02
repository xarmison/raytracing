#include "Line.hpp"
#include "Ray.hpp"

int main(int argc, char const *argv[]) {
    
    vec3d x(1, 2, 4), y(2.4, 5.7, 7.9);

    cout << "\nVec3D: " << (x - y).normalize() << endl;

    Ray b, n(x, y), m({0, 0, 0}, {1, 1, 1});

    cout << "\nRay: Origin: " << m.getOrigin() << " Dir: " << m.getDirection() << endl;
    
    Line a, s(x, y), d({0, 0, 0}, {1, 1, 1});
    
    return 0;
}
