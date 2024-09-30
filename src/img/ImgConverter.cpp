#include "img/ImgConverter.hpp"
#include "SFML/Graphics/Image.hpp"

#include <exception>
#include <iostream>
#include <cstdlib>
#include <tuple>

namespace img 
{

// Constructor
ImageConverter::ImageConverter(const std::string img_name)
    : img_name_(img_name)
{
}

/* MAIN FUNCTIONS */
void ImageConverter::load()
{
    try
    {
        load_img();
        load_RGB(false);
    }
    catch (const std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }
    
}

void ImageConverter::load_img()
{
    if (!img_.loadFromFile(get_name()))
    {
        throw std::runtime_error("Could not load image. Are you sure the path is correct?");
    }


    this->width = img_.getSize().x;
    this->height = img_.getSize().y;
}

void ImageConverter::load_RGB(bool debug = false)
{
    rgbMatrix_.resize(height, std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>(width));

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            sf::Color color = img_.getPixel(x, y);
            rgbMatrix_[y][x] = std::make_tuple(color.r, color.g, color.b);

            if (debug) {
                std::cout << "Pixel (" << x << ", " << y << "):" << std::endl;
                std::cout << "R = " << (int)(color.r) << std::endl;
                std::cout << "G = " << (int)(color.g) << std::endl;
                std::cout << "B = " << (int)(color.b) << std::endl;
            }
        }
    }
}



void ImageConverter::modify_RGB(float red, float green, float blue)
{
    if (rgbMatrix_.empty())
{
    throw std::runtime_error("RGB matrix is empty. Did you call load_RGB()?");
}

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            sf::Color color = img_.getPixel(x, y);
            rgbMatrix_[y][x] = std::make_tuple(
                static_cast<unsigned char>(color.r * red),
                static_cast<unsigned char>(color.g * green),
                static_cast<unsigned char>(color.b * blue)
            );
        }
    }
}

/* GETTERS */
const std::string ImageConverter::get_name() const
{
    return img_name_;
}

const RGBMatrix& ImageConverter::get_rgbMatrix() const 
{
    if (rgbMatrix_.empty())
    {
        throw std::runtime_error("RGB matrix is empty. Did you call load_RGB()?");
    }
    return rgbMatrix_;
}

int ImageConverter::get_width() const
{
    return width;
}

int ImageConverter::get_height() const
{
    return height;
}

}