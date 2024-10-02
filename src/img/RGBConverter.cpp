#include "img/RGBConverter.hpp"
#include <chrono>
#include <cstddef>


namespace img 
{

/* CONSTRUCTORS */
RGBConverter::RGBConverter()
{
    length_ = 0;
}

RGBConverter::RGBConverter(std::vector<RGBMatrix> &rgbMatrices)
    : rgbMatrices_(rgbMatrices)
{
    length_ = rgbMatrices.size();
}

/* MAIN FUNCTIONS */
ImgASCII RGBConverter::render_rgbMatrix(RGBMatrix& rgbMatrix)
{
    std::string asciiByBrightness = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/()1{}[]?-_+~<>i!lI;:,\"^`'.";
    float inv = asciiByBrightness.size() / 255.0f;

    // Initalizing the rendered ASCII image here is faster!
    ImgASCII renderedImgASCII(rgbMatrix.size());

    // Process each row
    for (int r = 0 ; r < rgbMatrix.size() ; r++)
    {
        renderedImgASCII[r].resize(rgbMatrix[r].size());
        for (int c = 0 ; c < rgbMatrix[r].size() ; c++)
        {
            auto pixel = rgbMatrix[r][c];

            // 0: R, 1: G, 2: B. Note that we perceive green as brighter colors.
            float brightness = 0.299f * std::get<0>(pixel) + 
                                0.587f * std::get<1>(pixel) + 
                                0.114f * std::get<2>(pixel);

            char asciiChar = asciiByBrightness[std::round(brightness*inv)];
            
            renderedImgASCII[r][c] = asciiByBrightness[static_cast<size_t>(brightness*inv)];
        }
    }
    return renderedImgASCII;
}


void RGBConverter::render_ascii()
{
    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "Processing images..." << std::endl;
    for (auto& rgbMatrix : rgbMatrices_)
        imgs_ascii_.push_back(render_rgbMatrix(rgbMatrix));
    std::cout << "Done!" << std::endl;

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    std::cout << "Time taken: " << duration.count() << " seconds" << std::endl;
}

/* GETTERS */
int RGBConverter::get_length() const
{
    return length_;
}

}