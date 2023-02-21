/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <unordered_map>

#include <TrueColorGIF/Encoder.hpp>


namespace TrueColorGIF {

struct Vec2D {
    Vec2D(unsigned int x_, unsigned int y_): x(x_), y(y_) {}

    auto operator<=>(const Vec2D&) const = default;

    unsigned int x;
    unsigned int y;
};

struct Palette {
    Palette() {
        colors.reserve(256ull * 3);     //Upper bound
        indices.reserve(65536);         //Estimation
        colorToIndexTable.reserve(256); //Upper bound
    }

    void clear() {
        colors.clear();
        indices.clear();
        colorToIndexTable.clear();
    }

    std::vector<uint8_t> colors;
    std::vector<uint8_t> indices;
    std::unordered_map<Color, unsigned int, ColorHasher> colorToIndexTable;
};

Vec2D constructPaletteFromRows(
    const Bitmap& bitmap,
    unsigned int startY,
    Palette& p
);

unsigned int constructPaletteFromRowRemainder(
    const Bitmap& bitmap,
    const Vec2D& start,
    Palette& p
);

}