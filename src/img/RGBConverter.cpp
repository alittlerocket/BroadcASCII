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
    std::string asciiByBrightness = ".'^:Il!i><~+_?][}{1)(/*#MW&8%B@$";
    float inv = (asciiByBrightness.size()-1) / 255.0f;

    // Faster to initialize it as such
    ImgASCII renderedImgASCII(rgbMatrix.size());

    // Process each row
    for (int r = 0; r < rgbMatrix.size(); ++r)
    {
        renderedImgASCII[r].resize(rgbMatrix[r].size());
        for (int c = 0; c < rgbMatrix[r].size(); ++c)
        {
            auto pixel = rgbMatrix[r][c];

            // Calculate perceived brightness (luminance)
            float brightness = 0.299f * std::get<0>(pixel) + 
                               0.587f * std::get<1>(pixel) + 
                               0.114f * std::get<2>(pixel);

            // Map the brightness to an ASCII character
            size_t index = static_cast<size_t>(brightness * inv);
            char asciiChar = asciiByBrightness[index];
            
            // Store the resulting ASCII character
            renderedImgASCII[r][c] = asciiChar;
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

const std::vector<ImgASCII> RGBConverter::get_ascii() const
{
    std::vector<ImgASCII> c = imgs_ascii_;
    return imgs_ascii_;
}

}