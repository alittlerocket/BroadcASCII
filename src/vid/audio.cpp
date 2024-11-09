#include<iostream>
#include<vid/audio.hpp>


namespace vid
{
    void play_audio(const std::string& input) {
        std::string audio_cmd = "ffplay -i " + input + " -vn -nodisp -autoexit -loglevel quiet";
        int result = system(audio_cmd.c_str());
        if (result != 0) {
            std::cerr << "Failed to play audio. Command exited with code " << result << std::endl;
        }
    }
}
