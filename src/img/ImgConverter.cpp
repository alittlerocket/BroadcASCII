#include "img/ImgConverter.hpp"
#include <algorithm>
#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavcodec/codec_par.h>
#include <libavutil/avutil.h>

namespace img {

ImageToASCIIConverter::ImageToASCIIConverter() 
{}

ImageToASCIIConverter::~ImageToASCIIConverter() 
{
    cleanup();
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

    return 0;
}

// Load the image
int ImageToASCIIConverter::load_image()
{
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
        throw ImageConversionException("Codec not found\n")
    }

    // Allocate and initialize the Codec Context
    codec_ctx = avcodec_alloc_context3(codec);
    if (!codec_ctx)
    {
        throw ImageConversionException("Could not allocate the video codec context\n");
    }
    avcodec_parameters_from_context(params, codec_ctx);


}

int find_stream_index(AVFormatContext *fmt_ctx, AVMediaType media_type)
{
    auto it = std::find_if(fmt_ctx->streams, fmt_ctx->streams+ fmt_ctx->nb_streams,
                                    [media_type](AVStream* stream) {
                                        return stream->codecpar->codec_type ==media_type;
                                    });
    return (it != fmt_ctx->streams + fmt_ctx->nb_streams) ? it - fmt_ctx->streams : -1;
}

}

