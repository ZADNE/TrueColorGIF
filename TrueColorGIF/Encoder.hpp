/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <vector>
#include <fstream>

namespace TrueColorGIF {

/**
 * @brief Is POD for decoded RGB, 8-bits-per-channel image
*/
struct Bitmap {
    unsigned int width;
    unsigned int height;
    std::vector<uint8_t> pixels;
};

/**
 * @brief Encodes bitmap to true-color GIF
*/
void encodeTrueColorGIF(std::ofstream& o, const Bitmap& bitmap);

}