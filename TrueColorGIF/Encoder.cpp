/*!
 *  @author    Dubsky Tomas
 */
#include <TrueColorGIF/Encoder.hpp>

#include <TrueColorGIF/FileOutput.hpp>

namespace TrueColorGIF {

void encodeImage(
    std::ofstream& o, const Bitmap& bitmap,
    unsigned int x, unsigned int y,
    unsigned int w, unsigned int h
);

void encodeTrueColorGIF(std::ofstream& o, const Bitmap& bitmap) {
    o << "GIF89a";                          //Header
    writeLittle<uint16_t>(o, bitmap.width); //Screen width
    writeLittle<uint16_t>(o, bitmap.height);//Screen height
    writeLittle<uint8_t>(o, 0b0'111'0'111); //No global color table, 24 bpp
    writeLittle<uint8_t>(o, 0);             //Unused background color index
    writeLittle<uint8_t>(o, 0);             //Default pixel aspect ratio

    //Encode the bitmap in 16x16 images
    for (int y = 0; y < bitmap.height; y += 16) {
        for (int x = 0; x < bitmap.width; x += 16) {
            auto w = std::min(16u, bitmap.width - x);
            auto h = std::min(16u, bitmap.height - y);
            encodeImage(                    //True color image
                o, bitmap,
                x, y,
                w, h
            );
        }
    }

    o << ';';                               //Trailer
}

void encodeImage(
    std::ofstream& o, const Bitmap& bitmap,
    unsigned int x, unsigned int y,
    unsigned int w, unsigned int h
) {
    o << ',';                               //Image separator
    writeLittle<uint16_t>(o, x);            //Image offset x
    writeLittle<uint16_t>(o, x);            //Image offset y
    writeLittle<uint16_t>(o, w);            //Image width
    writeLittle<uint16_t>(o, h);            //Image height
    writeLittle<uint8_t>(o, 0b1'0'0'00'111);//Local 256-unsorted-colors table

}

}