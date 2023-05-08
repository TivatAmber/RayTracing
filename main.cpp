#include "src/rtweekend.h"

#include "src/color.h"
#include "src/hittable_list.h"
#include "src/sphere.h"
#include "src/camera.h"

#include <iostream>
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0) return color (0, 0, 0);

    if (world.hit(r, 0, infinity, rec)) {
        point3 target = rec.p + rec.normal + random_in_unit_sphere();
        return 0.5 * ray_color(ray(rec.p, target - rec.p), world, depth - 1);
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
    const int samples_per_pixel = 100;
    const int max_depth = 10;

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera MainCamera;

    freopen("image.ppm", "w", stdout);

    // Draw
    std::cout << "P3" << std::endl << image_width << ' ' << image_height << std::endl << 255 << std::endl;

    for (int i = image_height - 1; i; i--) {
        std::cerr << "\rScanlines remaining: " << i << std::endl;
        for (int j = 0; j < image_width; j++) {
            color pixel_color (0.0, 0.0, 0.0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (i + random_double()) / (image_height - 1);
                auto v = (j + random_double()) / (image_width - 1);
                ray r = MainCamera.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone\n";
    return 0;
}
