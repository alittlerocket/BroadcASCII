#include <img/ImgConverter.hpp>

void test_ascii_conversion(std::string path)
{
    img::ImageToASCIIConverter c;

    img::ImgASCII ascii_image = c.convert(path);

    int count = 0;
    for (auto s : ascii_image)
    {
        for (char c : s) 
        {
            std::cout << c << " ";
        }
        std::cout << std::endl;
    }
}


int main() {
    try
    {
        std::cout << "Testing Ascii Conversion..." << std::endl;
        test_ascii_conversion("test_images/20x20_chicken.png");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}