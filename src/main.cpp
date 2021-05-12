#include <iomanip>
#include <iostream>

#include "../include/utility.h"

#include "../include/camera.h"
#include "../include/color.h"
#include "../include/hittable_list.h"
#include "../include/sphere.h"
#include "../include/material.h"
#include "../include/moving_sphere.h"

color ray_color(const ray &r, const hittable &world, int depth) {
    hit_record rec;

    // The ray exceeded the bounce limit, no more light is gathered
    if (depth <= 0)
        return color(0.0, 0.0, 0.0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;

        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);

        return color(0.0, 0.0, 0.0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    
    auto t = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;

    // auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    // world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, make_shared<lambertian>(checker)));

    for (int a = -5; a < 5; a++) {
        for (int b = -5; b < 5; b++) {
            auto choose_mat = random_double();
            point3 center(
                a + 0.9 * random_double(),
                0.2,
                b + 0.9 * random_double()
            );

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;
                
                if (choose_mat < 0.8) {
                    // Diffuse
                    auto albedo = color::random() * color::random();
                    
                    sphere_material = make_shared<lambertian>(albedo);

                    auto center_2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(make_shared<moving_sphere>(
                        center, center_2, 
                        0.0, 1.0,
                        0.2, sphere_material
                    )); 
                
                } else if (choose_mat < 0.95) {
                    // Metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0.0, 0.5);

                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));

                } else {
                    // Glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } 
            }

        }
    }

    // Big spheres
    world.add(make_shared<sphere>(
        point3(0, 1, 0),
        1.0,
        make_shared<dielectric>(1.5)
    ));

    world.add(make_shared<sphere>(
        point3(-4, 1, 0),
        1.0,
        make_shared<lambertian>(color(0.4, 0.2, 0.1))
    ));

    world.add(make_shared<sphere>(
        point3(4, 1, 0),
        1.0,
        make_shared<metal>(color(0.7, 0.6, 0.5), 0.0)
    ));

    return world;    
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(make_shared<sphere>(
        point3(0, -10, 0), 
        10, 
        make_shared<lambertian>(checker)
    ));
    
    objects.add(make_shared<sphere>(
        point3(0, 10, 0), 
        10, 
        make_shared<lambertian>(checker)
    ));

    return objects;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto perlin_text = make_shared<noise_texture>(4);

    objects.add(make_shared<sphere>(
        point3(0, -1000, 0), 
        1000, 
        make_shared<lambertian>(perlin_text)
    ));
    
    objects.add(make_shared<sphere>(
        point3(0, 2, 0), 
        2, 
        make_shared<lambertian>(perlin_text)
    ));

    return objects;
}

hittable_list earth() {
    auto globe = make_shared<sphere>(
        point3(0.0, 0.0, 0.0),
        2,
        make_shared<lambertian>(make_shared<image_texture>("../textures/earthmap.jpg"))
    );

    return hittable_list(globe);
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Missing Arguments!\nUsage: " << argv[0] << " scene_id\n";
        exit(-1);
    }

    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int im_width  = 1080;
    const int im_height = static_cast<int>(im_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    hittable_list world;

    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;

    int scene_id = atoi(argv[1]);
    switch (scene_id) {
        case 1:
            std::cerr << "Rendering scene 1\n";
            
            world = random_scene();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;

            break;
        
        case 2:
            std::cerr << "Rendering scene 2\n";

            world = two_spheres();
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;

            break;

        case 3:
            std::cerr << "Rendering scene 3\n";

            world = two_perlin_spheres();
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;

            break;

        case 4:
            std::cerr << "Rendering scene 4\n";
            
            world = earth();
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;

            break;

        default:
            std::cerr << "Scene id not found!\n";
            exit(-1);
            break;
    }

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

                pixel_color += ray_color(r, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nRendereing Done!\n";

    return 0;
}