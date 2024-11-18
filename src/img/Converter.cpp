/*
 * Converter.cpp
 *
 * This file contains the implementation of the Converter class, which is responsible
 * for converting video frames to ASCII representations. The class provides methods
 * to read video dimensions, scale video frames, and transform pixel data into ASCII
 * characters with color preservation.
 *
 * Key Features:
 * - Reads video dimensions using ffprobe and sets up ffmpeg for processing.
 * - Converts each frame of the video into an ASCII art representation.
 * - Allows scaling of frames to target dimensions and setting a target FPS.
 *
 * Dependencies:
 * - Requires ffmpeg and ffprobe for video processing.
 * - Relies on the `ColoredAscii` and `VidASCII` types for storing ASCII frame data.
 *
 * Author: Max Zhang
 * Date: November 17, 2024
 */

#include <img/Converter.hpp>

namespace img
{

// Constructor
Converter::Converter(std::string input_path, int target_fps, int target_width, int target_height)
    : _target_width(target_width), _target_height(target_height), _target_fps(target_fps)
{
    read_dimensions(input_path);
}


// Destructor
Converter::~Converter() {}


VidASCII Converter::convert()
{
    std::vector<uint8_t> frame_data(_target_width * _target_height * 3);
    VidASCII all_frames;

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) 
    {
        throw ConversionException("Failed to open pipe!");
    }

    // Read each frame, convert to ASCII, and store it
    while (fread(frame_data.data(), 1, frame_data.size(), pipe) == frame_data.size()) 
    {
        auto ascii_frame = convert_frame_to_ascii(frame_data, _target_width, _target_height);
        all_frames.push_back(ascii_frame);  // Store each frame
    }
    pclose(pipe);

    return all_frames;
}


// Getter for dimensions
std::pair<int, int> Converter::get_target_dimensions()
{
    std::pair<int, int> dimensions = {_target_width, _target_height};
    return dimensions;
}


// read the dimensions from the ffmpeg cli
void Converter::read_dimensions(std::string input)
{
    std::string probe_cmd = "ffprobe -v error -select_streams v:0 -show_entries stream=width,height -of csv=p=0 " + input;
    FILE* probe_pipe = popen(probe_cmd.c_str(), "r");
    if (!probe_pipe) 
    {
        throw ConversionException("Failed to open probe pipe!");
    }
    
    int width, height;
    fscanf(probe_pipe, "%d,%d", &width, &height);
    pclose(probe_pipe);
    
    // Set the corresponding class attributes
    cmd = "ffmpeg -i " + input + 
    " -vf \"scale=" + std::to_string(_target_width) + ":" + std::to_string(_target_height) 
    + ",fps=" + std::to_string(_target_fps) +
    + ",format=rgb24\" -f rawvideo -";
}


// pixel conversion
char Converter::pixel_to_ascii(uint8_t r, uint8_t g, uint8_t b)
{
    int brightness = static_cast<int>((0.3 * r) + (0.59 * g) + (0.11 * b));
    int index = (brightness * (ascii_chars.size() - 1)) / 255;
    return ascii_chars[index];
}

ImgASCII Converter::convert_frame_to_ascii(const std::vector<uint8_t>& frame_data, int width, int height)
{
    ImgASCII ascii_frame(height, std::vector<ColoredAscii>(width));
    int pixel_index = 0;
    for (int y = 0; y < height; ++y) 
    {
        for (int x = 0; x < width; ++x) 
        {
            uint8_t r = frame_data[pixel_index++];
            uint8_t g = frame_data[pixel_index++];
            uint8_t b = frame_data[pixel_index++];
            ascii_frame[y][x] = {pixel_to_ascii(r, g, b), r, g, b};
        }
    }
    return ascii_frame;
}

} // namespace img.

