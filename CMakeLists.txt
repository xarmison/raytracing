cmake_minimum_required(VERSION 3.0)
project(raytracing C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -g")

find_package(OpenMP REQUIRED)

file(GLOB HEADERS "*.h")

add_executable(test_img tests/test_img.cpp)
add_executable(pi tests/pi.cpp)
add_executable(mc_int tests/mc_integration.cpp)

add_executable(main src/main.cpp ${HEADERS})
target_link_libraries(main PRIVATE "${OpenMP_CXX_FLAGS}")
target_compile_options(main PRIVATE "${OpenMP_CXX_FLAGS}")

add_executable(cornell_box src/cornell_box.cpp ${HEADERS})