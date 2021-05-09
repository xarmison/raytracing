#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#include <iostream>

// Writes the translated [0,255] value of each color component
void write_color(std::ostream &out, color pixel_color) {
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}

#endif // COLOR_H