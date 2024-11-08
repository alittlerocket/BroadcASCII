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
        }
        std::cout << std::endl;
    }
}

void VidPlayer::play_video(const img::VidASCII& frames, int fps)
{
    int delay = 1000 / fps;

    for (const auto& frame : frames) 
    {
        display_ascii_frame(frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

}

