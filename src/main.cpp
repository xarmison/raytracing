#include <iostream>
#include <iomanip>

#include "../include/utility.h"
#include "../include/color.h"
#include "../include/hittable_list.h"
#include "../include/sphere.h"

// Returns the color of the background (a simple gradient)
double hit_sphere(const point3 &center, double radius, const ray &r) {
    vec3 oc = r.origin() - center;

    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius * radius;
    
    auto discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return -1.0;
    } else {
        return (-half_b - sqrt(discriminant)) / a;
    }
}

color ray_color(const ray &r) {
    double t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if (t > 0.0) {
        vec3 normal = unit_vector(r.at(t) - vec3(0, 0, -1));

        return 0.5 * color(
            normal.x() + 1,
            normal.y() + 1,
            normal.z() + 1
        );
    }

    vec3 unit_direction = unit_vector(r.direction());
    
    t = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[]) {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int im_width  = 400;
    const int im_height = static_cast<int>(im_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
   
    // Render
    std::cout << "P3\n" << im_width << " " << im_height << "\n255\n";

    std::cerr.precision(3);
    for (int j = im_height - 1; j >= 0; j--) {
        std::cerr << "Scanlines reamaining: " << std::setw(3) << j  
            << " of " << im_height 
            << " - " << std::setw(5) <<  static_cast<double>(im_height - j) / im_height * 100 << "%" 
        << '\r';
        
        for (int i = 0; i < im_width; i++) {
            auto u = double(i) / (im_width - 1);
            auto v = double(j) / (im_height - 1);
            
            ray r(
                origin,
                lower_left_corner + u * horizontal + v * vertical - origin            
            );

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::cerr << "\nRendereing Done!\n";

    return 0;
}