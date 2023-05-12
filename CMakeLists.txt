cmake_minimum_required(VERSION 3.25)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracing main.cpp src/vec3.h src/color.h src/ray.h src/hittable.h src/sphere.h src/hittable_list.h src/rtweekend.h src/camera.h src/material.h src/moving_sphere.h src/aabb.h src/aabb.h src/bvh.h src/texture.h src/perlin.h src/rtw_stb_image.h src/aarect.h)
