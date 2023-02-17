/*!
 *  @author    Dubsky Tomas
 */
#pragma once
#include <concepts>
#include <fstream>
#include <bit>
#include <algorithm>

namespace TrueColorGIF {

/**
 * @brief Reverses order of bytes in given value
*/
template<std::integral T>
constexpr T reverseByteOrder(const T& val) {
    union U {
        U(): raw() {}
        T val;
        std::array<std::byte, sizeof(T)> raw;
    };
    U source;
    source.val = val;
    U dest;

    std::reverse_copy(source.raw.begin(), source.raw.end(), dest.raw.begin());
    return dest.val;
}

namespace detail {
template<typename>inline constexpr bool dependent_false_v = false;
}

/**
 * @brief Writes an integral to outstream in little endianness
*/
template<std::integral T>
void writeLittle(std::ofstream& out, T i) {
    if constexpr (std::endian::native == std::endian::big) {
        out.write(reinterpret_cast<char*>(&reverseByteOrder(i)), sizeof(i));
    } else if constexpr (std::endian::native == std::endian::little) {
        out.write(reinterpret_cast<char*>(&i), sizeof(i));
    } else {
        static_assert(detail::dependent_false_v<T>, "Unsupported endianness!");
    }
}

}