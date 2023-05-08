//
// Created by Stellaris on 2023/5/8.
//

#ifndef RAYTRACING_RTWEEKEND_H
#define RAYTRACING_RTWEEKEND_H

#include <cmath>
#include <limits>

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

#include "ray.h"
#include "vec3.h"

#endif //RAYTRACING_RTWEEKEND_H
