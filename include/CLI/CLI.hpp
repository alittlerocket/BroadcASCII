#ifndef CLI_HPP
#define CLI_HPP

#include <img/Converter.hpp>
#include <vid/VidPlayer.hpp>


class CLI
{
    public:
        // Constructor
        CLI(const int argc, char* argv[]);
        
        // to execute the converter
        void run();

    private:
        std::string input_file;
        int fps = 30;
        int width, height;
        bool show_help = false;

        void print_help();

        void set_default_dim();

        void parse_arguments(const int argc, char* argv[]);    
};

#endif