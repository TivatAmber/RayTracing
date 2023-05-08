#include "src/rtweekend.h"

#include "src/color.h"
#include "src/hittable_list.h"
#include "src/sphere.h"

#include <iostream>
color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, infinity, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1);
    return (1.0 - t) * color (1.0, 1.0, 1.0) + t * color (0.5, 0.7, 1.0);
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

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

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
            color pixel_color = ray_color(r,world);
            write_color(std::cout, pixel_color);
        }
    }
    std::cerr << "\nDone\n";
    return 0;
}
