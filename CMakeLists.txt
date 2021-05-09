cmake_minimum_required(VERSION 3.0)
project(raytracing C CXX)

set(CMAKE_CXX_STANDARD 11)

set(HEADERS
    include/vec3.h
    include/color.h
    include/ray.h
    include/hittable.h
    include/sphere.h
    include/hittable_list.h
    include/utility.h
)

add_executable(test_img tests/test_img.cpp)

add_executable(main src/main ${HEADERS})