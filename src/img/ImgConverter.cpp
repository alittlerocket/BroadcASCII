#include "img/ImgConverter.hpp"


img::ImgASCII img::convertASCII(const sf::Image& img)
{
    int width = img.getSize().x;
    int height = img.getSize().y;

    // chars sorted by brightness
    std::string asciiByBrightness = ".'^:Il!i><~+_?][}{1)(/*#MW&8%B@$";
    float inv = (asciiByBrightness.size()-1) / 255.0f;

    // Faster to initialize this as such
    img::ImgASCII renderedImgASCII(height, std::string(width, ' '));

    for (int r = 0; r < height; r++)
    {
        for (int c = 0; c < width; c++)
        {
            // Get the color of the pixel.
            sf::Color color = img.getPixel(c, r);

            // Grab the index and set it at (r, c) by converting the colors
            // with the luminence formale and applying the inverse constant
            renderedImgASCII[r][c] = asciiByBrightness[
                                        static_cast<size_t>(
                                            color.r * 0.299f +
                                            color.g * 0.597f +
                                            color.b * 0.114f
                                        ) * inv];
        }
    }

    return renderedImgASCII;
}

img::ImgASCII img::to_ASCII(std::string path)
{
    sf::Image img;
    if (!img.loadFromFile(path))
    {
        throw std::runtime_error("Could not load image. Are you sure the path is correct?");
    }
    return img::convertASCII(img);
}