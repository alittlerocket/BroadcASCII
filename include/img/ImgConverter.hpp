#ifndef IMG_CONVERTER_HPP
#define IMG_CONVERTER_HPP

#include <iostream>
#include <algorithm>
#include <cstdint>
#include <vector>
#include <string>
#include <exception>


extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavcodec/codec.h>
    #include <libavcodec/codec_par.h>
    #include <libavcodec/packet.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libavutil/error.h>
    #include <libavutil/frame.h>
    #include <libavutil/pixfmt.h>
    #include <libswscale/swscale.h>
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
        std::vector<ImgASCII> convert(const std::string& image_path);

    private:
        AVFormatContext* fmt_ctx = nullptr;
        AVCodecContext* codec_ctx = nullptr;
        const AVCodec* codec = nullptr;
        int video_stream_index = -1;

        // Probabily will include more characters, but this is good enough.
        const std::string ascii_chars = "@%#*+=-:.";

        int open_input_file(const std::string& path);

        // Load the image file, and convert all images.
        std::vector<ImgASCII> load_images();

        // Convert the decoded image to grayscale
        AVFrame* convert_to_grayscale(AVFrame* frame);

        // Convert grayscale AVFrame to ASCII art
        ImgASCII frame_to_ascii(AVFrame* gray_frame);

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


int find_stream_index(AVFormatContext* fmt_ctx, AVMediaType media_type);

}

#endif
