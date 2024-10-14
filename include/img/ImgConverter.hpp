#ifndef IMG_CONVERTER_HPP
#define IMG_CONVERTER_HPP

#include <exception>
#include <iostream>
#include <chrono>
#include <tuple>


#include "SFML/Graphics/Image.hpp"


namespace img {

using ImgASCII = std::vector<std::string>;

// Converts the image to ascii
ImgASCII convertASCII(const sf::Image& img);

// Main image processing function
ImgASCII to_ASCII(std::string path);

}


#endif
