#include <iomanip>
#include <iostream>

#include "../include/utility.h"

#include "../include/camera.h"
#include "../include/color.h"
#include "../include/hittable_list.h"
#include "../include/sphere.h"
#include "../include/material.h"
#include "../include/moving_sphere.h"
#include "../include/aarect.h"
#include "../include/box.h"
#include "../include/constant_medium.h"
#include "../include/bvh.h"

color ray_color(const ray &r, const color &background, const hittable &world, int depth) {
    hit_record rec;

    // The ray exceeded the bounce limit, no more light is gathered
    if (depth <= 0)
        return color(0.0, 0.0, 0.0);

    // If the ray hits nothing, return the background color
    if (!world.hit(r, 0.001, infinity, rec)) 
        return background;

    ray scattered;
    color attenuation;
    color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);

    if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
        return emitted;

    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
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

hittable_list simple_light() {
    hittable_list objects;

    objects.add(make_shared<sphere>(
        point3(0, -1000, 0),
        1000,
        make_shared<lambertian>(color(0.5, 0.5, 0.5))
    ));

    objects.add(make_shared<sphere>(
        point3(0, 2, 0), 
        2, 
        make_shared<lambertian>(make_shared<noise_texture>(4))
    ));

    objects.add(make_shared<xy_rect>(
        3, 5, 1, 3, -2,
        make_shared<diffuse_light>(color(4.0, 4.0, 4.0))
    ));

    objects.add(make_shared<sphere>(
        point3(0, 7, 0),
        2,
        make_shared<diffuse_light>(color(4.0, 4.0, 4.0))
    ));

    return objects;
}

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));

    objects.add(make_shared<xz_rect>(213, 343, 227, 332, 554, light));

    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box_1 = make_shared<box>(
        point3(0.0, 0.0, 0.0),
        point3(165, 330, 165),
        white
    );
    box_1 = make_shared<rotate_y>(box_1, 15);
    box_1 = make_shared<translate>(box_1, vec3(265, 0, 295));
    objects.add(box_1);

    shared_ptr<hittable> box_2 = make_shared<box>(
        point3(0.0, 0.0, 0.0),
        point3(165, 165, 165),
        white
    );
    box_2 = make_shared<rotate_y>(box_2, -18);
    box_2 = make_shared<translate>(box_2, vec3(130, 0, 65));
    objects.add(box_2);

    return objects;
}

hittable_list cornell_box_smoke() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(7, 7, 7));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<xz_rect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), white);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));

    shared_ptr<hittable> box2 = make_shared<box>(point3(0,0,0), point3(165,165,165), white);
    box2 = make_shared<rotate_y>(box2, -18);
    box2 = make_shared<translate>(box2, vec3(130,0,65));

    objects.add(make_shared<constant_medium>(box1, 0.01, color(0,0,0)));
    objects.add(make_shared<constant_medium>(box2, 0.01, color(1,1,1)));

    return objects;
}

hittable_list final_scene() {
    hittable_list boxes1;
    auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w  = 100.0;
            
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;

            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(
                point3(x0, y0, z0),
                point3(x1, y1, z1),
                ground
            ));
        }
    }


    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    // Light
    objects.add(make_shared<xz_rect>(
        123, 423, 147, 412, 554, 
        make_shared<diffuse_light>(color(7, 7, 7))
    ));

    // Moving sphere
    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30, 0, 0);
    objects.add(make_shared<moving_sphere>(
        center1, center2, 
        0, 1, 50, 
        make_shared<lambertian>(color(0.7, 0.3, 0.1))
    ));

    // Glass sphere
    objects.add(make_shared<sphere>(
        point3(260, 150, 45), 
        50, 
        make_shared<dielectric>(1.5)
    ));

    // Metal sphere
    objects.add(make_shared<sphere>(
        point3(0, 150, 145), 
        50, 
        make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    //Fog
    auto boundary = make_shared<sphere>(
        point3(360,150,145), 
        70, 
        make_shared<dielectric>(1.5)
    );
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(
        boundary, 
        0.2, 
        color(0.2, 0.4, 0.9)
    ));
    boundary = make_shared<sphere>(
        point3(0, 0, 0), 
        5000, 
        make_shared<dielectric>(1.5)
    );
    objects.add(make_shared<constant_medium>(
        boundary, 
        .0001, 
        color(1,1,1)
    ));

    // Earth
    objects.add(make_shared<sphere>(
        point3(400, 200, 400),
        100,
        make_shared<lambertian>(make_shared<image_texture>("../textures/earthmap.jpg"))
    ));

    // Noise texture
    objects.add(make_shared<sphere>(
        point3(220, 280, 300),
        80,
        make_shared<lambertian>(
            make_shared<noise_texture>(0.1)
        )
    ));

    // Cube with random spheres
    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;

    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(
            point3::random(0, 165), 
            10,
            white
        ));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 
            15
        ),
        vec3(-100, 270, 395)
    ));

    return objects;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Missing Arguments!\nUsage: " << argv[0] << " scene_id\n";
        exit(-1);
    }

    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int im_width  = 1080;
    int samples_per_pixel = 100;
    int max_depth = 50;

    // World
    hittable_list world;

    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    auto background = color(0.0, 0.0, 0.0);

    int scene_id = atoi(argv[1]);
    switch (scene_id) {
        case 1:
            std::cerr << "Rendering scene 1\n";
            
            world = random_scene();
            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;

            break;
        
        case 2:
            std::cerr << "Rendering scene 2\n";

            world = two_spheres();
            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;

            break;

        case 3:
            std::cerr << "Rendering scene 3\n";

            world = two_perlin_spheres();
            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;

            break;

        case 4:
            std::cerr << "Rendering scene 4\n";
            
            world = earth();
            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;

            break;
        
        case 5:
            std::cerr << "Rendering scene 5\n";

            world = simple_light();
            background = color(0.0, 0.0, 0.0);
            lookfrom = point3(26.0, 3.0, 6.0);
            lookat = point3(0.0, 2.0, 0.0);
            vfov = 20.0;

            break;

        case 6:
            std::cerr << "Rendering scene 6\n";
            
            world = cornell_box();

            aspect_ratio = 1.0;
            im_width = 600;
            samples_per_pixel = 200;

            background = color(0.0, 0.0, 0.0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            
            break;

        case 7:
            std::cerr << "Rendering scene 7\n";
            
            world = cornell_box_smoke();

            aspect_ratio = 1.0;
            im_width = 600;
            samples_per_pixel = 200;

            background = color(0.0, 0.0, 0.0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            
            break;

        case 8:
            std::cerr << "Rendering scene 8\n";

            world = final_scene();

            aspect_ratio = 1.0;
            im_width = 800;
            samples_per_pixel = 10000;

            background = color(0.0, 0.0, 0.0);
            lookfrom = point3(478, 278, -600);
            lookat = point3(278, 278, 0);
            vfov = 40.0;

            break;

        default:
            std::cerr << "Scene id not found!\n";
            exit(-1);
            break;
    }

    int im_height = static_cast<int>(im_width / aspect_ratio);

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

                pixel_color += ray_color(r, background, world, max_depth);
            }

            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }

    std::cerr << "\nRendereing Done!\n";

    return 0;
}