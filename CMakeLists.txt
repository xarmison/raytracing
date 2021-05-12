cmake_minimum_required(VERSION 3.0)
project(raytracing C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(HEADERS
    ./include/aabb.h
    ./include/bvh.h
    ./include/camera.h
    ./include/color.h
    ./include/external/stb_image.h
    ./include/external/stb_image_write.h
    ./include/hittable.h
    ./include/hittable_list.h
    ./include/material.h
    ./include/moving_sphere.h
    ./include/perlin.h
    ./include/ray.h
    ./include/sphere.h
    ./include/texture.h
    ./include/utility.h
    ./include/vec3.h
)

add_executable(test_img tests/test_img.cpp)

add_executable(main src/main ${HEADERS})