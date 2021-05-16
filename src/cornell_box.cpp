#include <iomanip>
#include <iostream>

#include "../include/utility.h"
#include "../include/timer.h"

#include "../include/camera.h"
#include "../include/color.h"
#include "../include/hittable_list.h"
#include "../include/sphere.h"
#include "../include/material.h"
#include "../include/aarect.h"
#include "../include/box.h"
#include "../include/bvh.h"
#include "../include/pdf.h"

color ray_color(const ray &r, const color &background, const hittable &world, const shared_ptr<hittable>& lights, int depth) {
    hit_record rec;

    // The ray exceeded the bounce limit, no more light is gathered
    if (depth <= 0)
        return color(0.0, 0.0, 0.0);

    // If the ray hits nothing, return the background color
    if (!world.hit(r, 0.001, infinity, rec)) 
        return background;

    scatter_record srec;
    color emitted = rec.mat_ptr->emitted(r, rec, rec.u, rec.v, rec.p);
    if (!rec.mat_ptr->scatter(r, rec, srec))
        return emitted;

    if (srec.is_specular) {
        return srec.attenuation
            * ray_color(srec.specular_ray, background, world, lights, depth - 1);
    }

    auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
    mixture_pdf p(light_ptr, srec.pdf_ptr);

    ray scattered = ray(rec.p, p.generate(), r.time());
    auto pdf_val = p.value(scattered.direction());

    return emitted + srec.attenuation 
        * rec.mat_ptr->scattering_pdf(r, rec, scattered)
        * ray_color(scattered, background, world, lights, depth - 1) / pdf_val;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));

    objects.add(make_shared<flip_face>(
        make_shared<xz_rect>(213, 343, 227, 332, 554, light)
    ));

    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<hittable> box1 = make_shared<box>(
        point3(0, 0, 0), 
        point3(165, 330, 165),
        aluminum
    );
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265, 0, 295));
    objects.add(box1);

    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(point3(190, 90, 190), 90, glass));

    return objects;
}

int main(int argc, char* argv[]) {

    // Image
    auto aspect_ratio = 1.0;
    int im_width  = 1080;
    int im_height = static_cast<int>(im_width / aspect_ratio);
    int samples_per_pixel = 200;
    int max_depth = 50;

    // World
    hittable_list world = cornell_box();
    auto lights = make_shared<hittable_list>();
    lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
    lights->add(make_shared<sphere>(point3(190, 90, 190), 90, shared_ptr<material>()));

    point3 lookfrom = point3(278, 278, -800);
    point3 lookat = point3(278, 278, 0);
    auto vfov = 40.0;
    auto aperture = 0.0;
    auto background = color(0.0, 0.0, 0.0);

    std::cerr << "Rendering Cornell Box\n";
            
    // Camera
    vec3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;

    camera cam(
        lookfrom, lookat, vup, 
        vfov, aspect_ratio, 
        aperture, dist_to_focus,
        0.0, 1.0
    );

    // Render
    std::cout << "P3\n" << im_width << " " << im_height << "\n255\n";

    std::cerr.precision(3);
    for (int j = im_height - 1; j >= 0; j--) {
        
        auto start = std::chrono::high_resolution_clock::now();
    
        for (int i = 0; i < im_width; i++) {
            
            color pixel_color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = double(i + random_double()) / (im_width - 1);
                auto v = double(j + random_double()) / (im_height - 1);
                
                ray r = cam.get_ray(u, v);

                pixel_color += ray_color(r, background, world, lights, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }

        auto end = std::chrono::high_resolution_clock::now();
    
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        
        std::cerr << "Scanlines reamaining: " << std::setw(3) << j  
            << " of " << im_height 
            << " - " << std::setw(6) <<  static_cast<double>(im_height - j) / im_height * 100 << "%" 
        << " - ETA: "  << time_remaining(elapsed, j) << '\r';
    }

    std::cerr << "\nRendereing Done!\n";

    return 0;
}