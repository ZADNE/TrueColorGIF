/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <TrueColorGIF/Encoder.hpp>

namespace TrueColorGIF {

class Collagist {
public:
    Collagist() {}

    Collagist(const Bitmap& bitmap);

    void addImage(const Vec2D& offset, const Vec2D& size);

    unsigned int saveCollage(const std::string& filename);

private:
    Bitmap m_collage;
    uint32_t m_rng = 0u;
    unsigned int m_imageCount = 0u;
};

}