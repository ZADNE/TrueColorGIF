/*!
 *  @author    Dubsky Tomas
 */
#include <TrueColorGIF/Compression.hpp>

namespace TrueColorGIF {

struct Code {
    Code(unsigned int bits_, unsigned int data_):
        bits(bits_), data(data_) {
    }

    unsigned int bits;/**< Number of valid bits in data */
    unsigned int data;/**< Data of the code */
};

class CodeSerializer {
public:
    CodeSerializer(std::vector<uint8_t>& output):
        m_output(output) {
    }

    CodeSerializer(const CodeSerializer&) = delete;
    CodeSerializer& operator=(const CodeSerializer&) = delete;

    CodeSerializer(CodeSerializer&&) = delete;
    CodeSerializer& operator=(CodeSerializer&&) = delete;

    void write(const Code& code) {
        m_remainder.data |= code.data << m_remainder.bits;
        m_remainder.bits += code.bits;
        while (m_remainder.bits >= 8) {
            m_output.push_back(m_remainder.data);
            m_remainder.data >>= 8;
            m_remainder.bits -= 8;
        }
    }

    ~CodeSerializer() {
        if (m_remainder.bits > 0) {
            m_remainder.data &= (1 << m_remainder.bits) - 1;//Clear last bits
            m_output.push_back(m_remainder.data);
        }
    }

private:
    Code m_remainder{0u, 0u};
    std::vector<uint8_t>& m_output;
};

void compressWithLZW(
    std::vector<uint8_t>& output,
    unsigned int initialCodeSize,
    const std::vector<uint8_t>& input
) {
    output.clear();
    const Code clearCode{initialCodeSize + 1, 1u << initialCodeSize};
    CodeSerializer o{output};

    o.write(clearCode);
    size_t i;
    size_t clearInterval = std::max((1ull << initialCodeSize) - 2, 1ull);
    for (i = 0; i < (input.size() % clearInterval); i++) {
        o.write(Code{initialCodeSize + 1, input[i]});
    }

    for (; i < input.size(); i += clearInterval) {
        o.write(clearCode);
        for (size_t j = 0; j < clearInterval; j++) {
            o.write(Code{initialCodeSize + 1, input[i + j]});
        }
    }

    o.write(Code{initialCodeSize + 1, clearCode.data + 1}); //Stop code
}

}