/*!
 *  @author    Dubsky Tomas
 */
#include <TrueColorGIF/Collagist.hpp>

#include <lodepng/lodepng.hpp>

namespace TrueColorGIF {

uint32_t xorshift(uint32_t x) {
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

Collagist::Collagist(const Bitmap& bitmap):
    m_collage(bitmap),
    m_rng(1984258863){
}

void Collagist::addImage(const Vec2D& offset, const Vec2D& size) {
    Color c{
        .r = static_cast<uint8_t>(m_rng >> 16),
        .g = static_cast<uint8_t>(m_rng >> 8),
        .b = static_cast<uint8_t>(m_rng)
    };
    m_rng = xorshift(m_rng);

    for (unsigned int y = offset.y; y < (offset.y + size.y); y++) {
        for (unsigned int x = offset.x; x < (offset.x + size.x); x++) {
            m_collage.color(x, y) = c;
        }
    }
}

unsigned int Collagist::saveCollage(const std::string& filename) {
    return lodepng::encode(
        filename, m_collage.pixels.data(),
        m_collage.width, m_collage.height,
        LodePNGColorType::LCT_RGB, 8u
    );
}

}