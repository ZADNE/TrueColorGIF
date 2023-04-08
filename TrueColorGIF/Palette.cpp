/*!
 *  @author    Dubsky Tomas
 */
#include <TrueColorGIF/Palette.hpp>

namespace TrueColorGIF {

unsigned int addRowToPalette(
    const Bitmap& bitmap,
    const Vec2D& start,
    Palette& p
) {
    for (unsigned int x = start.x; x < bitmap.width; x++) {
        const Color& col = bitmap.color(x, start.y);
        auto it = p.colorToIndexTable.find(col);
        if (it != p.colorToIndexTable.end()) {
            //The color has already been in the palette
            p.indices.push_back(it->second);
        } else if (p.colorToIndexTable.size() <= 255) {
            //Insert new color into the palette
            auto index = static_cast<unsigned int>(p.colorToIndexTable.size());
            p.colorToIndexTable.emplace_hint(it, std::make_pair(col, index));
            p.colors.push_back(col.r);
            p.colors.push_back(col.g);
            p.colors.push_back(col.b);
            p.indices.push_back(index);
        } else {
            //New color has been encountered but the palette is full
            return x - start.x;
        }
    }
    return bitmap.width - start.x;
}

Vec2D constructPaletteFromRows(
    const Bitmap& bitmap,
    unsigned int startY,
    Palette& p
) {
    p.clear();
    for (unsigned int y = startY; y < bitmap.height; y++) {
        auto prevColorsSize = p.colors.size();
        auto prevIndicesSize = p.indices.size();
        auto covered = addRowToPalette(bitmap, Vec2D{0u, y}, p);
        if (covered < bitmap.width) {
            //Could not cover the whole row
            if (y == startY) {
                //Could not cover the first row
                return Vec2D{covered, 1u};
            } else {
                //Could not cover 2nd or later row
                //Reverse vectors to their state at the end of the previous row
                p.colors.resize(prevColorsSize);
                p.indices.resize(prevIndicesSize);
                return Vec2D{bitmap.width, y - startY};
            }
        }
    }
    //Successfully covered the whole area
    return Vec2D{bitmap.width, bitmap.height - startY};
}

unsigned int constructPaletteFromRowRemainder(const Bitmap& bitmap, const Vec2D& start, Palette& p) {
    p.clear();
    return addRowToPalette(bitmap, start, p);
}

}