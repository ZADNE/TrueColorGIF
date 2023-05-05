/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <vector>
#include <cstdint>


namespace TrueColorGIF {

void compressWithLZW(
    std::vector<uint8_t>& output,
    unsigned int dataCodeSize,
    const std::vector<uint8_t>& input
);

}