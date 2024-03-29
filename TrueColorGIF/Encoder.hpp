﻿/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <vector>
#include <fstream>
#include <functional>

namespace TrueColorGIF {

struct Color {
    auto operator<=>(const Color&) const = default;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct ColorHasher {
    std::size_t operator()(const Color& c) const {
        return (c.b << 16) | (c.g << 8) | (c.r);
    }
};

/**
 * @brief Is POD for decoded RGB, 8-bits-per-channel image
*/
struct Bitmap {

    const Color& color(unsigned int x, unsigned int y) const {
        return *reinterpret_cast<const Color*>(&pixels[(y * width + x) * 3u]);
    }

    Color& color(unsigned int x, unsigned int y) {
        return *reinterpret_cast<Color*>(&pixels[(y * width + x) * 3u]);
    }

    unsigned int width = 0u;
    unsigned int height = 0u;
    std::vector<uint8_t> pixels;
};

struct Vec2D {
    Vec2D(unsigned int x_, unsigned int y_): x(x_), y(y_) {}

    auto operator<=>(const Vec2D&) const = default;

    unsigned int x;
    unsigned int y;
};

using ImageCallback = std::function<void(const Vec2D& offset, const Vec2D& size)>;

/**
 * @brief Encodes bitmap to true-color GIF
*/
void encodeTrueColorGIF(std::ofstream& o, const Bitmap& bitmap, const ImageCallback& callback);

}