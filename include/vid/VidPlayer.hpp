#ifndef VIDPLAYER_HPP
#define VIDPLAYER_HPP

#include <iostream>
#include <thread>
#include <chrono>
#include "img/Converter.hpp"

namespace vid 
{

class VidPlayer
{
    public:
        // Constructor
        VidPlayer();

        // Destructor
        ~VidPlayer();

        // Play video
        void play_video(const img::VidASCII& frames, int fps);

    private:
        // Function to convert and print each individual frame
        void display_ascii_frame(const img::ImgASCII& ascii_frame);

};

} 

#endif