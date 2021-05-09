#include <iomanip>
#include <iostream>

#include "../include/camera.h"
#include "../include/color.h"
#include "../include/hittable_list.h"
#include "../include/sphere.h"
#include "../include/utility.h"

color ray_color(const ray &r, const hittable &world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1.0, 1.0, 1.0));
    }

    vec3 unit_direction = unit_vector(r.direction());
    
    auto t = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main(int argc, char* argv[]) {

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int im_width  = 400;
    const int im_height = static_cast<int>(im_width / aspect_ratio);
    const int samples_per_pixel = 100;

    // World
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera cam;

    // Render
    std::cout << "P3\n" << im_width << " " << im_height << "\n255\n";

    std::cerr.precision(3);
    for (int j = im_height - 1; j >= 0; j--) {
        std::cerr << "Scanlines reamaining: " << std::setw(3) << j  
            << " of " << im_height 
            << " - " << std::setw(5) <<  static_cast<double>(im_height - j) / im_height * 100 << "%" 
        << '\r';

        for (int i = 0; i < im_width; i++) {
            
            color pixel_color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = double(i + random_double()) / (im_width - 1);
                auto v = double(j + random_double()) / (im_height - 1);
                
                ray r = cam.get_ray(u, v);

                pixel_color += ray_color(r, world);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nRendereing Done!\n";

    return 0;
}