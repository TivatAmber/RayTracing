#include "vec3.h"
#include "ray.h"
#include "color.h"

#include <iostream>

bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 oc = r.orig - center;
    auto a = dot(r.direction(), r.direction());
    auto b = 2.0 * dot(oc, r.direction());
    auto c = dot(oc, oc) - radius * radius;
    auto discriminant = b * b - 4 * a * c;
    return discriminant > 0;
}

color ray_color(const ray& r) {
    if (hit_sphere(point3(0, 0, -1), 0.5, r))
        return color(1, 0, 0);
    vec3 unit_direction = unit_vector(r.dire);
    auto t = 0.5 * (unit_direction.y()  + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 1920;
    const int image_height = static_cast<int>(image_width / aspect_ratio);

    // Camera
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * aspect_ratio;
    auto focal_length = 1.0;

    // Screen
    auto origin = point3(0, 0, 0);
    auto horizontal = vec3(viewport_width, 0, 0);
    auto vertical = vec3(0, viewport_height, 0);
    auto lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);

    freopen("image.ppm", "w", stdout);

    // Draw
    std::cout << "P3" << std::endl << image_width << ' ' << image_height << std::endl << 255 << std::endl;

    for (int i = image_height - 1; i; i--) {
        std::cerr << "\rScanlines remaining: " << i << std::endl;
        for (int j = 0; j < image_width; j++) {
            auto u = double(i) / (image_height - 1);
            auto v = double(j) / (image_width - 1);
            ray r(origin, lower_left_corner + v * horizontal + u * vertical - origin);
            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone\n";
    return 0;
}
