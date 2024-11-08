#include <iostream>
#include <vector>
#include <chrono>
#include <thread>

#include <img/Converter.hpp>

void display_ascii_frame(const img::ImgASCII& ascii_frame) {
    for (const auto& row : ascii_frame) {
        for (const auto& pixel : row) {
            std::cout << "\033[38;2;" << static_cast<int>(pixel.r) << ";" 
                      << static_cast<int>(pixel.g) << ";" 
                      << static_cast<int>(pixel.b) << "m" 
                      << pixel.ascii_char << "\033[0m";
        }
        std::cout << std::endl;
    }
}

// Function to play all frames at a given frame rate
void play_video(const img::VidASCII& frames, int fps) {
    int delay = 1000 / fps;

    for (const auto& frame : frames) {
        display_ascii_frame(frame);
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
}

int main() {
    img::Converter converter = img::Converter(50, "test_files/bad_apple.mp4");
    img::VidASCII ascii_video = converter.convert();

    int fps = 30;

    play_video(ascii_video, fps);
    return 0;
}
