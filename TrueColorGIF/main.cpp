/*!
 *  @author    Dubsky Tomas
 */
#include <filesystem>
#include <iostream>
#include <fstream>

#include <lodepng/lodepng.hpp>

#include <TrueColorGIF/Encoder.hpp>


int main(int argc, char* argv[]) {
    //Parse and validate command line arguments
    if (argc < 3) {
        std::cerr << "You need to specify input file and output file!\n";
        return 1;
    }
    std::filesystem::path inFilename = argv[1];
    std::filesystem::path outFilename = argv[2];

    //Decode input file
    TrueColorGIF::Bitmap bitmap;
    if (inFilename.extension() == ".png") {
        //Decode the PNG
        auto error = lodepng::decode(
            bitmap.pixels, bitmap.width, bitmap.height,
            inFilename.string(),
            LodePNGColorType::LCT_RGB, 8u
        );
        if (error) {
            std::cerr << "Failed to decode the PNG file: " << lodepng_error_text(error) << '\n';
            return 1;
        }
    } else {
        std::cerr << "Input file is expected to have '.png' extension!\n";
        return 1;
    }

    try {
        //Prepare output file
        std::ofstream outFile{outFilename, std::ios_base::binary | std::ios_base::trunc};
        outFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        //Encode the GIF
        TrueColorGIF::encodeTrueColorGIF(outFile, bitmap);

    } catch (std::exception& e) {
        std::cerr << "Failed to encode GIF: " << e.what() << '\n';
        return 1;
    }

    //Success
    return 0;
}