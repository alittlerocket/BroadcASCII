#include <vid/VidPlayer.hpp>

namespace vid 
{

// Constructor
VidPlayer::VidPlayer() {};

// Desstructor
VidPlayer::~VidPlayer() {};

void VidPlayer::display_ascii_frame(const img::ImgASCII& ascii_frame) 
{
    for (const auto& row : ascii_frame) 
    {
        for (const auto& pixel : row) 
        {
            std::cout << "\033[38;2;" << static_cast<int>(pixel.r) << ";" 
                    << static_cast<int>(pixel.g) << ";" 
                    << static_cast<int>(pixel.b) << "m" 
                    << pixel.ascii_char << "\033[0m";

            // std::cout << pixel.ascii_char;
        }
        std::cout << std::endl;
    }
}


void VidPlayer::play_video(const img::VidASCII& frames, int fps)
{
    using clock = std::chrono::steady_clock;
    auto frame_duration = std::chrono::microseconds(1'000'000 / fps);
    auto next_frame_time = clock::now();

    for (const auto& frame : frames) 
    {
        display_ascii_frame(frame);

        // Wait until the next frame's time
        next_frame_time += frame_duration;
        std::this_thread::sleep_until(next_frame_time);
    }
}

}

