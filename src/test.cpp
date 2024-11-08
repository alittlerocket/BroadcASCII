#include <iostream>
#include <cstdint>

char pixel_to_ascii(int r, int g, int b)
{
    const std::string ascii_chars = "@%#*+=-:.";
    int brightness = static_cast<int>((0.3 * r) + (0.59 * g) + (0.11 * b));
    int index = (brightness * (ascii_chars.size() - 1)) / 255;
    std::cout << "Brightness: " << brightness << ", Index: " << index << std::endl;
    return ascii_chars[index];
}

int main() {
    std::cout << "Test character for RGB(255, 255, 255): " << pixel_to_ascii(255, 255, 255) << std::endl;
    std::cout << "Test character for RGB(0, 0, 0): " << pixel_to_ascii(0, 0, 0) << std::endl;
    std::cout << "Test character for RGB(128, 128, 128): " << pixel_to_ascii(128, 128, 128) << std::endl;
    return 0;
}