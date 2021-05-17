#include <iomanip>
#include <iostream>
#include <omp.h>
#include <string>
#include <vector>

#include "../include/utility.h"
#include "../include/timer.h"

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

hittable_list cornell_box() {
    hittable_list objects;

    auto red   = make_shared<lambertian>(color(.65, .05, .05));
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    auto green = make_shared<lambertian>(color(.12, .45, .15));
    auto light = make_shared<diffuse_light>(color(15, 15, 15));

    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));

    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));

    shared_ptr<material> aluminum = make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
    shared_ptr<hittable> box1 = make_shared<box>(point3(0,0,0), point3(165,330,165), aluminum);
    box1 = make_shared<rotate_y>(box1, 15);
    box1 = make_shared<translate>(box1, vec3(265,0,295));
    objects.add(box1);

    auto glass = make_shared<dielectric>(1.5);
    objects.add(make_shared<sphere>(point3(190,90,190), 90 , glass));

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

    objects.add(make_shared<flip_face>(make_shared<xz_rect>(213, 343, 227, 332, 554, light)));
    
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
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1,101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<box>(point3(x0,y0,z0), point3(x1,y1,z1), ground));
        }
    }

    hittable_list objects;

    objects.add(make_shared<bvh_node>(boxes1, 0, 1));

    auto light = make_shared<diffuse_light>(color(7, 7, 7));
    objects.add(make_shared<flip_face>(make_shared<xz_rect>(123, 423, 147, 412, 554, light)));

    auto center1 = point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto moving_sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
    objects.add(make_shared<moving_sphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<sphere>(point3(260, 150, 45), 50, make_shared<dielectric>(1.5)));
    objects.add(make_shared<sphere>(
        point3(0, 150, 145), 50, make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = make_shared<sphere>(point3(360,150,145), 70, make_shared<dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
    boundary = make_shared<sphere>(point3(0,0,0), 5000, make_shared<dielectric>(1.5));
    objects.add(make_shared<constant_medium>(boundary, .0001, color(1,1,1)));

    auto emat = make_shared<lambertian>(make_shared<image_texture>("../textures/earthmap.jpg"));
    objects.add(make_shared<sphere>(point3(400,200,400), 100, emat));
    auto pertext = make_shared<noise_texture>(0.1);
    objects.add(make_shared<sphere>(point3(220,280,300), 80, make_shared<lambertian>(pertext)));

    hittable_list boxes2;
    auto white = make_shared<lambertian>(color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<sphere>(point3::random(0,165), 10, white));
    }

    objects.add(make_shared<translate>(
        make_shared<rotate_y>(
            make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
            vec3(-100,270,395)
        )
    );

    return objects;
}

int main(int argc, char* argv[]) {

    if (argc < 2) {
        std::cerr << "Missing Arguments!\nUsage: " << argv[0] << " scene_id\n";
        exit(-1);
    }

    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int im_width  = 400;
    int samples_per_pixel = 100;
    int max_depth = 50;

    // World
    hittable_list world;

    auto lights = make_shared<hittable_list>();
    lights->add(make_shared<xz_rect>(213, 343, 227, 332, 554, shared_ptr<material>()));
    lights->add(make_shared<sphere>(point3(190, 90, 190), 90, shared_ptr<material>()));

    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    auto background = color(0.0, 0.0, 0.0);

    int scene_id = atoi(argv[1]);
    switch (scene_id) {
        case 1:
            std::cerr << "Rendering random spheres scene\n";
            
            world = random_scene();

            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;

            break;
        
        case 2:
            std::cerr << "Rendering two spheres scene\n";

            world = two_spheres();

            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;

            break;

        case 3:
            std::cerr << "Rendering scene with two perlin noise textures spheres\n";

            world = two_perlin_spheres();

            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;

            break;

        case 4:
            std::cerr << "Rendering earth scene\n";
            
            world = earth();

            background = color(0.70, 0.80, 1.00);
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;

            break;

        case 5:
            std::cerr << "Rendering cornell box scene\n";
            
            world = cornell_box();

            aspect_ratio = 1.0;
            im_width = 600;
            samples_per_pixel = 100;

            background = color(0.0, 0.0, 0.0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            
            break;

        case 6:
            std::cerr << "Rendering cornell box with smoke objects\n";
            
            world = cornell_box_smoke();

            aspect_ratio = 1.0;
            im_width = 600;
            samples_per_pixel = 200;

            background = color(0.0, 0.0, 0.0);
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40.0;
            
            break;

        case 7:
            std::cerr << "Rendering final scene\n";

            world = final_scene();

            lights->add(make_shared<xz_rect>(
                123, 423, 147, 412, 554, 
                make_shared<diffuse_light>(color(7, 7, 7))
            ));

            aspect_ratio = 1.0;
            im_width = 1000;
            samples_per_pixel = 100;

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

    // Show progress controls
    auto total_steps = im_height;
    size_t steps_completed = 0;

    // Stores the rednered image 
    std::vector<std::vector<std::string>> image(im_height, std::vector<std::string>(im_width));

    #pragma omp parallel for num_threads(8) 
    for (int j = im_height - 1; j >= 0; j--) {
        #pragma omp atomic
        ++steps_completed;

        #pragma omp critical
        std::cerr << "Scanlines done: " << std::setw(3) << steps_completed  
            << " of " << total_steps 
            << " - " << std::setw(6) << std::setprecision(3) << 100.0 * steps_completed / total_steps << "%" 
        << '\r';   
                
        for (int i = 0; i < im_width; i++) {
            
            color pixel_color(0, 0, 0);

            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = double(i + random_double()) / (im_width - 1);
                auto v = double(j + random_double()) / (im_height - 1);

                
                ray r = cam.get_ray(u, v);

                pixel_color += ray_color(r, background, world, lights, max_depth);
            }

            #pragma omp critical
            image[im_height - (j + 1)][i] = get_color(pixel_color, samples_per_pixel);
        }   
    }

    std::cerr << "\nRendereing Done!\n";

    for(int row = 0; row < image.size(); row++) 
        for (int col = 0; col < image[row].size(); col++) 
            std::cout << image[row][col] << '\n';

    std::cerr << "Done!\n";

    
    return 0;
}