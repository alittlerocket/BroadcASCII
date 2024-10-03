#ifndef RGB_CONVERTER_HPP
#define RGB_CONVERTER_HPP

#include <exception>
#include <iostream>
#include <chrono>
#include <cmath>
#include <tuple>

#include "SFML/Graphics/Image.hpp"

namespace img {

using RGBMatrix = std::vector<std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>>;
using ImgASCII = std::vector<std::string>;

class RGBConverter 
{
    public:
        // Constructors
        RGBConverter();
        RGBConverter(std::vector<RGBMatrix> &rgbMatrices_);

        // Renders the ascii art images
        void render_ascii();

        // // add the matrix to the end of rgbMatrices_
        // void append_matrix();

        // Getters
        const std::vector<ImgASCII> get_ascii() const;
        int get_length() const;
    
    private:
        ImgASCII render_rgbMatrix(RGBMatrix& rgbMatrix);

        std::vector<RGBMatrix> rgbMatrices_;
        std::vector<ImgASCII> imgs_ascii_;
        int length_;
        
};

}

#endif