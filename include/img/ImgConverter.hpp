#ifndef IMG_CONVERTER_HPP
#define IMG_CONVERTER_HPP

#include <iostream>
#include <cstdint>
#include <vector>
#include <string>
#include <exception>

// FFMpeg types forward declaration
extern "C" {
    #include <libavformat/avformat.h>
    #include <libavcodec/avcodec.h>
    #include <libswscale/swscale.h>
    #include <libavutil/imgutils.h>
}

namespace img {

using ImgASCII = std::vector<std::string>;

class ImageToASCIIConverter {
    public:
        // Constructor
        ImageToASCIIConverter();

        // Destructor
        ~ImageToASCIIConverter();

        // Function to convert an image from file to ASCII art
        ImgASCII convert(const std::string& image_path);

    private:
        AVFormatContext* format_ctx = nullptr;
        AVCodecContext* codec_ctx = nullptr;
        AVFrame* frame = nullptr;
        SwsContext* sws_ctx = nullptr;
        int video_stream_index = -1;

        // Probabily will include more characters, but this is good enough.
        const std::string ascii_chars = "@%#*+=-:. ";

        // Load the image file using FFmpeg
        bool load_image(const std::string& path);

        // Convert the decoded image to grayscale
        AVFrame* convert_to_grayscale();

        // Convert grayscale AVFrame to ASCII art
        ImgASCII frame_to_ascii(const AVFrame* gray_frame);

        // Map a grayscale pixel value to an ASCII character
        char pixel_to_ascii(uint8_t pixel_value);

        // Cleanup function to free resources
        void cleanup();
};

// Custom exception.
class ImageConversionException : public std::exception {
    public:
        explicit ImageConversionException(const std::string& message) : msg_(message) {}

        const char* what() const noexcept override {
            return msg_.c_str();
        }

    private:
        std::string msg_;
};

}

#endif
