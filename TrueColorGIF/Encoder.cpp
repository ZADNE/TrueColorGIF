/*!
 *  @author    Dubsky Tomas
 */
#include <TrueColorGIF/Encoder.hpp>
#include <TrueColorGIF/FileOutput.hpp>
#include <TrueColorGIF/Compression.hpp>
#include <TrueColorGIF/Palette.hpp>

namespace TrueColorGIF {

void encodeImage(
    std::ofstream& o,
    const Vec2D& start, const Vec2D& size,
    const Palette& palette,
    std::vector<uint8_t>& s
);

void encodeTrueColorGIF(std::ofstream& o, const Bitmap& bitmap) {
    o << "GIF89a";                              //Header
    writeLittle<uint16_t>(o, bitmap.width);     //Screen width
    writeLittle<uint16_t>(o, bitmap.height);    //Screen height
    writeLittle<uint8_t>(o, 0b0'111'0'111);     //No global color table, 24 bpp
    writeLittle<uint8_t>(o, 0);                 //Unused background color index
    writeLittle<uint8_t>(o, 0);                 //Default pixel aspect ratio

    //Encode the bitmap
    Palette p;
    std::vector<uint8_t> s;
    s.reserve(65536);
    Vec2D cursor{0u, 0u};                       //Points to first pixel of next image
    while (cursor != Vec2D{0u, bitmap.height}) {
        if (cursor.x == 0u) {
            //Try to cover full rows
            auto covered = constructPaletteFromRows(bitmap, cursor.y, p);
            encodeImage(o, cursor, covered, p, s);
            if (covered.x == bitmap.width) {//Covered full rows
                cursor.y += covered.y;
            } else {//Could not cover full row
                cursor.x += covered.x;
            }
        } else {
            //Try to finish the remainder of this row
            auto covered = constructPaletteFromRowRemainder(bitmap, cursor, p);
            encodeImage(o, cursor, Vec2D{covered, 1u}, p, s);
            cursor.x += covered;
            if (cursor.x == bitmap.width) {
                //Finished this row
                cursor = Vec2D{0u, cursor.y + 1u};
            }
        }
    }

    o << ';';                                   //Trailer
}

void encodeImage(
    std::ofstream& o,
    const Vec2D& start, const Vec2D& extent,
    const Palette& p,
    std::vector<uint8_t>& s
) {
    //Determine size of the local color table
    size_t tableSize =
        std::max(std::bit_ceil<size_t>(p.colors.size() / 3ull), 2ull);
    unsigned int indexBits = std::countr_zero(tableSize) - 1;
    unsigned int flags = 0b1'0'0'00'000 | indexBits;

    //Graphic control extension
    o << '!';                                   //Extension introducer
    writeLittle<uint8_t>(o, 0xF9);              //Graphic control label
    writeLittle<uint8_t>(o, 4);                 //Block size
    writeLittle<uint8_t>(o, 0b000'001'0'0);     //Keep the image, no tranparent color
    writeLittle<uint16_t>(o, 2);                //20 ms delay - apparently minimum delay in web browsers 
    writeLittle<uint8_t>(o, 0);                 //Unused transparent color index
    o << '\0';                                  //Image data block terminator

    //Image descriptor
    o << ',';                                   //Image separator
    writeLittle<uint16_t>(o, start.x);          //Image offset x
    writeLittle<uint16_t>(o, start.y);          //Image offset y
    writeLittle<uint16_t>(o, extent.x);         //Image width
    writeLittle<uint16_t>(o, extent.y);         //Image height
    writeLittle<uint8_t>(o, flags);             //Local unsorted-colors table

    //Local color table
    o.write(reinterpret_cast<const char*>(p.colors.data()), tableSize * 3ull);

    //Image data
    unsigned int initCodeSize = indexBits + 1;
    writeLittle<uint8_t>(o, initCodeSize);      //Initial code size

    //Compress the color indices with LZW encoding
    compressWithLZW(s, initCodeSize, p.indices);

    size_t bytesWritten = 0ull;
    size_t streamSize = s.size();
    while (bytesWritten < s.size()) {           //Separate the datastream into blocks
        size_t blockSize = std::min(streamSize - bytesWritten, 255ull);
        writeLittle<uint8_t>(o, blockSize);     //Size of this data block
        o.write(                                //The data block
            reinterpret_cast<const char*>(&s[bytesWritten]), blockSize
        );
        bytesWritten += blockSize;
    }

    o << '\0';                                  //Image data block terminator
}

}