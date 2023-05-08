//
// Created by Stellaris on 2023/5/8.
//

#ifndef RAYTRACING_RAY_H
#define RAYTRACING_RAY_H

#include "vec3.h"

class ray {
public:
    ray() {}
    ray(const point3& origin, const vec3 direction):orig(origin), dire(direction) {}

    point3 origin() const {return orig;}
    vec3 direction() const {return dire;}

    point3 at(double t) const {
        return orig + t * dire;
    }

private:
    point3 orig;
    vec3 dire;
};

#endif //RAYTRACING_RAY_H
