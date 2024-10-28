#include "img/ImgConverter.hpp"
#include <libavutil/frame.h>
#include <libavutil/imgutils.h>
#include <libavutil/pixfmt.h>
#include <vector>


namespace img {

ImageToASCIIConverter::ImageToASCIIConverter() 
{}

ImageToASCIIConverter::~ImageToASCIIConverter() 
{
    cleanup();
}

// Convert to greyscale
AVFrame* ImageToASCIIConverter::convert_to_grayscale(AVFrame* frame)
{
    SwsContext* sws_ctx = sws_getContext(
        frame->width, frame->height, codec_ctx->pix_fmt, 
        frame->width, frame->height, AV_PIX_FMT_GRAY8,
        SWS_BILINEAR, nullptr, nullptr, nullptr);

    AVFrame* gray_frame = av_frame_alloc();
    gray_frame->width = frame->width;
    gray_frame->height = frame->height;
    gray_frame->format = AV_PIX_FMT_GRAY8;

    int buffer_size = av_image_get_buffer_size(AV_PIX_FMT_GRAY8, frame->width, frame->height, 1);
    std::vector<uint8_t> buffer(buffer_size);
    av_image_fill_arrays(gray_frame->data, gray_frame->linesize, buffer.data(), AV_PIX_FMT_GRAY8, frame->width, frame->height, 1);

    sws_scale(sws_ctx, frame->data, frame->linesize, 0, frame->height, gray_frame->data, gray_frame->linesize);
    return gray_frame;
}

// Convert the pixel to the associated character.
char ImageToASCIIConverter::pixel_to_ascii(uint8_t pixel_value) {
    int index = (pixel_value * (ascii_chars.size() - 1)) / 255; // Scale to ascii_chars range
    return ascii_chars[index];
}

ImgASCII ImageToASCIIConverter::frame_to_ascii(AVFrame* frame) {
    ImgASCII ascii_art;
    AVFrame* gray_frame = convert_to_grayscale(frame);

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

// Take the input and open it given the path
int ImageToASCIIConverter::open_input_file(const std::string& path) 
{
    if (avformat_open_input(&fmt_ctx, path.c_str(), nullptr, nullptr) != 0)
    {
        throw ImageConversionException("Cannot open input file: " + path);
    }

    if (avformat_find_stream_info(fmt_ctx, nullptr) != 0)
    {
        throw ImageConversionException("Cannot find stream information for file: " + path);
    }

    // Find the first video stream
    int video_stream_index = find_stream_index(fmt_ctx, AVMEDIA_TYPE_VIDEO);
    if (video_stream_index == -1)
    {
        throw ImageConversionException("No video stream detected.");
    }

    // Find the appropriate codec
    AVCodecParameters* params = fmt_ctx->streams[video_stream_index]->codecpar;
    codec = avcodec_find_decoder(params->codec_id);
    if (!codec)
    {
        throw ImageConversionException("Codec not found\n");
    }

    // Allocate and initialize the Codec Context
    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx)
    {
        throw ImageConversionException("Could not allocate the video codec context\n");
    }
    avcodec_parameters_from_context(params, codec_ctx);

    // Open codec
    if (avcodec_open2(codec_ctx, codec, nullptr) < 0)
    {
        throw ImageConversionException("Could not open the codec\n");
    }
    return 0;
}

// Load the image
std::vector<ImgASCII> ImageToASCIIConverter::load_images()
{
    /* Steps:
    *   1) Find the first video stream from fmt_ctx and store index    
    *   2) Find the appropriate codec.
    *   3) Allocate and initialize the codec context
    *   4) Open the codec.
    */
    std::vector<ImgASCII> ascii_art_images;

    AVFrame* frame = av_frame_alloc();
    AVPacket* packet = av_packet_alloc();

    if (!frame || !packet)
    {
        throw ImageConversionException("could not allocate frame/packet");
    }

    // Read packets
    while (av_read_frame(fmt_ctx, packet) >= 0)
    {
        if (packet->stream_index == video_stream_index)
        {
            int ret;
            if (ret < 0)
            {
                throw ImageConversionException("Error sending packet to decoder\n");
                av_packet_unref(packet);
                break;
            }

            while (ret >= 0)
            {
                ret = avcodec_receive_frame(codec_ctx, frame);
                
                // We're done, no more frames
                if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                {
                    break;
                }
                else if (ret < 0) 
                {
                    throw ImageConversionException("Error receiving frame from decoder\n");
                    break;
                }

                ascii_art_images.push_back(frame_to_ascii(frame));
                std::cout << "Decoded the frame with dimensions: " << frame->width << "x" << frame->height << std::endl;

                // Deref frame.
                av_frame_unref(frame);
            }
        }
        av_packet_unref(packet);
    }

    return ascii_art_images;
}

std::vector<ImgASCII> ImageToASCIIConverter::convert(const std::string& image_path)
{
    open_input_file(image_path);
    return load_images();
}

int find_stream_index(AVFormatContext *fmt_ctx, AVMediaType media_type)
{
    auto it = std::find_if(fmt_ctx->streams, fmt_ctx->streams+ fmt_ctx->nb_streams,
                                    [media_type](AVStream* stream) {
                                        return stream->codecpar->codec_type ==media_type;
                                    });
    return (it != fmt_ctx->streams + fmt_ctx->nb_streams) ? it - fmt_ctx->streams : -1;
}

void ImageToASCIIConverter::cleanup() {
    if (codec_ctx) {
        avcodec_free_context(&codec_ctx); // Free the codec context
        codec_ctx = nullptr;
    }

    if (fmt_ctx) {
        avformat_close_input(&fmt_ctx);   // Close the input file and free the format context
        fmt_ctx = nullptr;
    }
}

}






