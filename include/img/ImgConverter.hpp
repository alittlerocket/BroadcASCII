#ifndef IMG_CONVERTER_HPP
#define IMG_CONVERTER_HPP

#include <exception>
#include <iostream>

#include "SFML/Graphics/Image.hpp"
#include <cstdlib>
#include <tuple>


namespace img {

using RGBMatrix = std::vector<std::vector<std::tuple<unsigned char, unsigned char, unsigned char>>>;

class ImageConverter 
{
public:
    // Constructor
    ImageConverter(const std::string img_name);

    // Public method to load image and populate the RGB matrix
    void load();

    // You can modify or "weight" the rgb values
    void modify_RGB(float red, float green, float blue);

    // Getters
    const std::string get_name() const;
    const RGBMatrix& get_rgbMatrix() const;
    int get_height() const;
    int get_width() const;
    
private:
    // Also the img path
    std::string img_name_;

    // The actual image object from SFML
    sf::Image img_;

    // Dimensions
    int width_;
    int height_;

    // RGBMatrix, needs to be loaded.
    RGBMatrix rgbMatrix_;

    void load_img();
    void load_RGB(bool debug);
};

}

#endif
