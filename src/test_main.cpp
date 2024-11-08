#include <vid/VidPlayer.hpp>
#include <img/Converter.hpp>

int main() {
    img::Converter ascii_converter = img::Converter(50, "test_files/bad_apple.mp4");
    vid::VidPlayer vid_player = vid::VidPlayer();

    img::VidASCII ascii_video = ascii_converter.convert();
    int fps = 30;

    vid_player.play_video(ascii_video, fps);
    return 0;
}
