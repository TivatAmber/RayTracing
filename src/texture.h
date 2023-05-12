//
// Created by BYU on 2023/5/11.
//

#ifndef RAYTRACING_TEXTURE_H
#define RAYTRACING_TEXTURE_H

#include "rtweekend.h"
#include "rtw_stb_image.h"

#include "perlin.h"

class texture {
public:
    virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color: public texture {
public:
    solid_color() {}
    solid_color(color c): color_value(c) {}

    solid_color(double red, double green, double blue)
    : solid_color(color(red, green, blue)) {}

    virtual color value(double u, double v, const point3& p) const override {
        return color_value;
    }

private:
    color color_value;
};

class checker_texture: public texture {
public:
    checker_texture();
    checker_texture(std::shared_ptr<solid_color> _even, std::shared_ptr<solid_color> _odd):
    even(_even), odd(_odd) {}

    checker_texture(color c1, color c2):
    even(std::make_shared<solid_color>(c1)), odd(std::make_shared<solid_color>(c2)) {}

    virtual color value(double u, double v, const point3& p) const override {
        auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0) return odd->value(u, v, p);
        else return even->value(u, v, p);
    }

public:
    std::shared_ptr<solid_color> even;
    std::shared_ptr<solid_color> odd;
};

class noise_texture : public texture {
public:
    noise_texture() {}
    noise_texture(double sc):scale(sc) {}

    virtual color value(double u, double v, const point3& p) const override {
        return color (1, 1, 1) * (1 + sin(10 * noise.turb(p) + scale * p.z())) * 0.5;
    }
public:
    perlin noise;
    double scale;
};

class image_texture: public texture {
public:
    const static int bytes_per_pixel = 3;

    image_texture():
        data(nullptr), width(0), height(0), bytes_per_scanline(0) {}

    image_texture(const char* filename) {
        auto components_per_pixel = bytes_per_scanline;

        data = stbi_load(filename, &width, &height, &components_per_pixel, bytes_per_pixel);

        if (!data) {
            std::cerr << "ERROR: Could not load the file '" << filename << "'.\n";
            width = height = 0;
        }

        std::cout << width << " " << height << std::endl;

        bytes_per_scanline = bytes_per_pixel * width;
    }

    ~image_texture() {delete data;}

    virtual color value(double u, double v, const point3& p) const override {
        if (data == nullptr) return color (0.0, 1.0, 1.0);

        u = clamp(u, 0.0, 1.0);
        v = 1.0 - clamp(v, 0.0, 1.0);

        auto i = static_cast<int>(u * width);
        auto j = static_cast<int>(v * height);

        if (i >= width) i = width - 1;
        if (j >= height) j = height - 1;

//        std::cerr << i << " " << j << "\n";

        const auto scale_color = 1.0 / 255.0;
        auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

        return color (pixel[0] * scale_color, pixel[1] * scale_color, pixel[2] * scale_color);
    }
private:
    unsigned char *data;
    int width, height;
    int bytes_per_scanline;
};

#endif //RAYTRACING_TEXTURE_H
