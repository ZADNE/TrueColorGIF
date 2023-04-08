/*!
 *  @author    Dubsky Tomas
 */
#include <filesystem>
#include <iostream>
#include <fstream>

#include <lodepng/lodepng.hpp>

#include <TrueColorGIF/Encoder.hpp>
#include <TrueColorGIF/Collagist.hpp>


int main(int argc, char* argv[]) {
    //Parse and validate command line arguments
    if (argc < 3) {
        std::cerr << "You need to specify input file and output file!\n";
        return 1;
    }
    std::filesystem::path inFilename = argv[1];
    std::filesystem::path outFilename = argv[2];

    //Parse options
    bool showCollage = false;
    TrueColorGIF::Collagist collagist;
    for (int i = 3; i < argc; i++) {
        if (std::strcmp(argv[i], "-c") == 0) {
            showCollage = true;
        }
    }

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

    if (showCollage) {
        collagist = TrueColorGIF::Collagist{bitmap};
    }

    try {
        //Prepare output file
        std::ofstream outFile{outFilename, std::ios_base::binary | std::ios_base::trunc};
        outFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        using namespace std::placeholders;
        TrueColorGIF::ImageCallback callback = std::bind(&TrueColorGIF::Collagist::addImage, &collagist, _1, _2);

        //Encode the GIF
        TrueColorGIF::encodeTrueColorGIF(outFile, bitmap, showCollage ? callback : nullptr);

    } catch (std::exception& e) {
        std::cerr << "Failed to encode GIF: " << e.what() << '\n';
        return 1;
    }

    if (showCollage) {
        auto error = collagist.saveCollage(outFilename.string() + ".png");
        if (error) {
            std::cerr << "Failed to decode the PNG file: " << lodepng_error_text(error) << '\n';
            return 1;
        }
    }

    //Success
    return 0;
}