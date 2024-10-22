#include "img/ImgConverter.hpp"

namespace img {

ImageToASCIIConverter::ImageToASCIIConverter() 
{}

ImageToASCIIConverter::~ImageToASCIIConverter() 
{
    cleanup();
}

bool ImageToASCIIConverter::load_image(const std::string& path) {
    if (avformat_open_input(&format_ctx, path.c_str(), nullptr, nullptr) != 0) {
        std::cerr << "Error: Could not open input file." << std::endl;
        return false;
    }

    // Retrieve stream information
    if (avformat_find_stream_info(format_ctx, nullptr) < 0) {
        std::cerr << "Error: Could not find stream information." << std::endl;
        return false;
    }

    // Find video stream
    const AVCodec* codec = nullptr;
    for (unsigned int i = 0; i < format_ctx->nb_streams; i++) {
        if (format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) 
        {
            video_stream_index = i;
            codec = avcodec_find_decoder(format_ctx->streams[i]->codecpar->codec_id);
            break;
        }
    }

    if (!codec) {
        std::cerr << "Error: Codec not found." << std::endl;
        return false;
    }

    // Allocate codec context
    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx) {
        std::cerr << "Error: Could not allocate codec context." << std::endl;
        return false;
    }

    // Copy codec parameters from stream
    if (avcodec_parameters_to_context(codec_ctx, format_ctx->streams[video_stream_index]->codecpar) < 0) {
        std::cerr << "Error: Could not copy codec parameters to codec context." << std::endl;
        return false;
    }

    // Open codec
    if (avcodec_open2(codec_ctx, codec, nullptr) < 0) {
        std::cerr << "Error: Could not open codec." << std::endl;
        return false;
    }

    // Allocate frame to store the decoded image
    frame = av_frame_alloc();

    // Read the first frame
    AVPacket packet;
    while (av_read_frame(format_ctx, &packet) >= 0) {
        if (packet.stream_index == video_stream_index) {
            if (avcodec_send_packet(codec_ctx, &packet) == 0) {
                if (avcodec_receive_frame(codec_ctx, frame) == 0) {
                    av_packet_unref(&packet);
                    return true;
                }
            }
        }
        av_packet_unref(&packet);
    }

    std::cerr << "Error: Could not decode frame." << std::endl;
    return false;
}

// Convert the decoded image to grayscale
AVFrame* ImageToASCIIConverter::convert_to_grayscale() {
    sws_ctx = sws_getContext(
        frame->width, frame->height, codec_ctx->pix_fmt,  // Source width, height, and pixel format
        frame->width, frame->height, AV_PIX_FMT_GRAY8,    // Target pixel format: grayscale (8-bit)
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    // Allocate memory for grayscale image
    AVFrame* gray_frame = av_frame_alloc();
    int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_GRAY8, frame->width, frame->height, 1);
    std::vector<uint8_t> buffer(buffer_size);
    av_image_fill_arrays(gray_frame->data, gray_frame->linesize, buffer.data(), AV_PIX_FMT_GRAY8, frame->width, frame->height, 1);

    // Convert the frame to grayscale
    sws_scale(sws_ctx, frame->data, frame->linesize, 0, frame->height, gray_frame->data, gray_frame->linesize);

    return gray_frame;
}

// Convert the grayscale AVFrame to ASCII art
ImgASCII ImageToASCIIConverter::frame_to_ascii(const AVFrame* gray_frame) {
    ImgASCII ascii_art;

    for (int y = 0; y < gray_frame->height; ++y) {
        std::string line;
        for (int x = 0; x < gray_frame->width; ++x) {
            uint8_t pixel_value = gray_frame->data[0][y * gray_frame->linesize[0] + x];
            line += pixel_to_ascii(pixel_value); 
        }
        ascii_art.push_back(line); 
    }

    return ascii_art;
}

// Map a grayscale pixel value to an ASCII character
char ImageToASCIIConverter::pixel_to_ascii(uint8_t pixel_value) {
    int index = (pixel_value * (ascii_chars.size() - 1)) / 255;
    return ascii_chars[index];
}

// Convert image file to ASCII art
ImgASCII ImageToASCIIConverter::convert(const std::string& image_path) {
    if (!load_image(image_path)) {
        throw ImageConversionException("Failed to load image: " + image_path);
    }
    AVFrame* gray_frame = convert_to_grayscale();
    if (!gray_frame) {
        throw ImageConversionException("Failed to convert image to grayscale.");
    }
    ImgASCII ascii_art = frame_to_ascii(gray_frame);
    av_frame_free(&gray_frame);
    return ascii_art;
}

// Free allocated resources
void ImageToASCIIConverter::cleanup() {
    if (frame) {
        av_frame_free(&frame);
    }
    if (codec_ctx) {
        avcodec_free_context(&codec_ctx);
    }
    if (format_ctx) {
        avformat_close_input(&format_ctx);
    }
    if (sws_ctx) {
        sws_freeContext(sws_ctx);
    }
}

} // namespace img
