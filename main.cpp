#include "src/rtweekend.h"

#include "src/color.h"
#include "src/hittable_list.h"
#include "src/sphere.h"
#include "src/camera.h"
#include "src/material.h"
#include "src/moving_sphere.h"

#include <iostream>
color ray_color(const ray& r, const hittable& world, int depth) {
    hit_record rec;

    if (depth <= 0) return color (0, 0, 0);

    if (world.hit(r, 0.001, infinity, rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return color (0, 0, 0);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto t = 0.5 * (unit_direction.y() + 1);
    return (1.0 - t) * color (1.0, 1.0, 1.0) + t * color (0.5, 0.7, 1.0);
}

hittable_list random_scene() {
    hittable_list world;

    auto checker = std::make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    world.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, std::make_shared<lambertian>(checker)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0, 0.5), 0);
                    world.add(std::make_shared<moving_sphere>(center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    return world;
}

hittable_list two_spheres() {
    hittable_list objects;

    auto checker = std::make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));

    objects.add(std::make_shared<sphere>(point3(0,-10, 0), 10, std::make_shared<lambertian>(checker)));
    objects.add(std::make_shared<sphere>(point3(0, 10, 0), 10, std::make_shared<lambertian>(checker)));

    return objects;
}

hittable_list two_perlin_spheres() {
    hittable_list objects;

    auto pertext = std::make_shared<noise_texture>(4);
    objects.add(std::make_shared<sphere>(point3(0,-1000,0), 1000, std::make_shared<lambertian>(pertext)));
    objects.add(std::make_shared<sphere>(point3(0, 2, 0), 2, std::make_shared<lambertian>(pertext)));

    return objects;
}

hittable_list earth() {
    auto earth_texture = std::make_shared<image_texture>("earthmap.jpg");
    auto earth_surface = std::make_shared<lambertian>(earth_texture);
    auto globe = std::make_shared<sphere>(point3(0,0,0), 2, earth_surface);

    return hittable_list(globe);
}

int main() {
    // Image
    const auto aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    const int max_depth = 50;

    // World
    hittable_list world;
    point3 lookfrom;
    point3 lookat;
    auto vfov = 40;
    auto aperture = 0.0;

    switch (4) {
        case 1:
            world = random_scene();
            lookfrom = point3 (13, 2, 3);
            lookat = point3 (0, 0, 0);
            vfov = 20.0;
            aperture = 0.1;
            break;

        case 2:
            world = two_spheres();
            lookfrom = point3 (13, 2, 3);
            lookat = point3 (0, 0, 0);
            vfov = 20.0;
            break;

        case 3:
            world = two_perlin_spheres();
            lookfrom = point3 (13, 2, 3);
            lookat = point3 (0, 0, 0);
            vfov = 20.0;
            break;

        case 4:
            world = earth();
            lookfrom = point3 (-13, 2, 3);
            lookat = point3 (0, 0, 0);
            vfov = 20.0;
            break;
    }

    // Camera
    vec3 vup(0, 1, 0);
    auto focus_dist = 10;

    camera MainCamera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, focus_dist, 0.0, 1.0);

    freopen("image.ppm", "w", stdout);

    // Draw
    std::cout << "P3" << std::endl << image_width << ' ' << image_height << std::endl << 255 << std::endl;

    for (int i = image_height - 1; i; i--) {
        std::cerr << "\rScanlines remaining: " << i << std::endl;
        for (int j = 0; j < image_width; j++) {
            color pixel_color (0.0, 0.0, 0.0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (j + random_double()) / (image_width - 1);
                auto v = (i + random_double()) / (image_height - 1);
                ray r = MainCamera.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
    }
    std::cerr << "\nDone\n";
    return 0;
}
