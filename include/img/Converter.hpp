#ifndef CONVERTER_HPP
#define CONVERTER_HPP

#include <cstdint>
#include <vector>
#include <string>

namespace img 
{

struct ColoredAscii
{
    char ascii_char;
    uint8_t r,g,b;
};

using PixelASCII = std::vector<ColoredAscii>;
using ImgASCII = std::vector<PixelASCII>;
using VidASCII = std::vector<ImgASCII>;

class Converter 
{
    public:
        // Constructor
        Converter(std::string input_path, int fps, int target_width = 80, int target_height = 24);

        // Destructor
        ~Converter();

        // Main conversion function
        VidASCII convert();

        // Getter for dimensions
        std::pair<int, int> get_target_dimensions();

    private:
        // dimensions
        int _target_width = 80;
        int _target_height = 24;

        // fps
        int _target_fps;
        
        // FFmpeg command still yet to be determined. Needs width and height for proper scaling.
        std::string cmd;
        
        // Characters that will be used in the actual ascii image
        const std::string ascii_chars = ".:-=+*#%@";

        // read the dimensions from the ffmpeg cli
        void read_dimensions(std::string input);

        // pixel conversion
        char pixel_to_ascii(uint8_t r, uint8_t g, uint8_t b);
        
        // process all the characters and store them
        ImgASCII convert_frame_to_ascii(const std::vector<uint8_t>& frame_data, int width, int height);
};

// Custom exception.
class ConversionException : public std::exception {
    public:
        explicit ConversionException(const std::string& message) : msg_(message) {}

        const char* what() const noexcept override {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};

}

#endif