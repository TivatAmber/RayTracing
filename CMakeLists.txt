cmake_minimum_required(VERSION 3.25)
project(RayTracing)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracing main.cpp vec3.h color.h ray.h)
