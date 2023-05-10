//
// Created by BYU on 2023/5/8.
//

#ifndef RAYTRACING_CAMERA_H
#define RAYTRACING_CAMERA_H

#include "rtweekend.h"

class camera {
public:
    camera(
            point3 lookfrom,
            point3 lookat,
            vec3 vup,
            double fov,
            double aspect_ratio,
            double aperture,
            double focus_dist
            ) {
        // Camera
        auto theta = degrees_to_radians(fov);
        auto h = tan(theta / 2);
        auto viewport_height = 2.0 * h;
        auto viewport_width = viewport_height * aspect_ratio;

        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        auto focal_length = 1.0;

        origin = lookfrom;
        horizontal = viewport_width * u * focus_dist;
        vertical = viewport_height * v * focus_dist;
        lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist * w;

        lens_radius = aperture / 2;
    }

    ray get_ray(double s, double t) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + t * horizontal + s * vertical - origin - offset);
    }
private:
    point3 origin;
    point3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    double lens_radius;
};

#endif //RAYTRACING_CAMERA_H
