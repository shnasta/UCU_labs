// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com


#include "image_maker.hpp"
#include <png++/png.hpp>
#include <tinycolormap.hpp>

#include <Magick++.h>

image_maker::image_maker(int height, int width, int num_frames) :
    height(height), width(width){
    images.resize(num_frames);
};


png::rgb_pixel doubleToColor(double value) {
    // Map the double value to the range [0, 255]
    const tinycolormap::Color color = tinycolormap::GetColor(value, tinycolormap::ColormapType::Heat);

    // Map the intValue to different intensities for each color channel
    return {static_cast<unsigned char>(color.r()*255), static_cast<unsigned char>(color.g()*255),
            static_cast<unsigned char>(color.b()*255)};
}


void image_maker::write_image(const std::string& filename, const std::vector<double>& table) {
    png::image<png::rgb_pixel> image(width, height);

    if (table.size() != static_cast<size_t>(width) * height) {
        std::cerr << "Error: Invalid vector size. Expected " << width * height << " elements." << std::endl;;
    }

    int index = 0;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double value = table[index++];
            image[y][x] = doubleToColor(value);
        }
    }

    image.write(filename);
    images[num_frames++] = filename;
}

void image_maker::write_gif(const std::string& filename) {
    std::cout << "Generating GIF..." << std::endl;

    if (images.empty()) {
        std::cerr << "Error: No input images to generate GIF." << std::endl;
        return;
    }

    try {
        Magick::InitializeMagick(nullptr);
        std::vector<Magick::Image> frames;

        for (const auto& inputFile : images) {
            Magick::Image image(inputFile);
            image.resize(Magick::Geometry(width, height));
            image.magick("GIF");
//            image.animationDelay(1000);
            frames.push_back(image);
        }

        for (auto& frame : frames) {
            frame.animationDelay(5);
        }

        Magick::writeImages(frames.begin(), frames.end(), filename);

        std::cout << "GIF generated and saved to '" << filename << "'." << std::endl;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << std::endl;
    }
}
