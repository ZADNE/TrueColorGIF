/*!
 *  @author    Dubsky Tomas
 */
#include <TrueColorGIF/Encoder.hpp>

#include <TrueColorGIF/FileOutput.hpp>

namespace TrueColorGIF {

void encodeTrueColorGIF(std::ofstream& o, const Bitmap& bitmap) {
    o << "GIF89a";

    uint16_t a = 256;

    writeLittle(o, a);
}

}