/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <vector>


namespace TrueColorGIF {

void compressWithLZW(
    std::vector<uint8_t>& output,
    unsigned int initialCodeSize,
    const std::vector<uint8_t>& input
);

}