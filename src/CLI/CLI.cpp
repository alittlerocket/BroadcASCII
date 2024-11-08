#include <exception>
#include <getopt.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <CLI/CLI.hpp>

CLI::CLI(const int argc, char* argv[])
{
    set_default_dim();
    parse_arguments(argc, argv);
}

void CLI::print_help()
{
    std::cout << 
        "Usage: broadcascii [OPTION...] <input-video>\n" <<
        "Converts the input video to ascii art with ANSI color codes\n" <<
        "\t-h, --help                       display this help and exit\n" <<
        "\t-f, --fps <desired_fps>          change the fps value to <desired_fps>\n" <<
        "\t-w, --width <target_width>       sets the target_width to <target_width>. Default is current terminal width\n" <<
        "\t-h, --height <target_height>     sets the target_height to <targeT_height>. Default is current terminal height\n" <<
    std::endl;
}

void CLI::set_default_dim()
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        std::cerr << "ioctl" << std::endl;
        width = 80;
        height = 24;
    }
    
    width = w.ws_col;
    height = w.ws_row;
}

void CLI::parse_arguments(int argc, char* argv[])
{
    struct option long_options[] = {
            {"help", no_argument, nullptr, 'h'},
            {"fps", required_argument, nullptr, 'f'},
            {"width", required_argument, nullptr, 'w'},
            {"height", required_argument, nullptr, 'l'},
            {nullptr, 0, nullptr, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "hf:w:l:", long_options, nullptr)) != -1) {
        switch (opt) 
        {
            case 'h':
                show_help = true;
                break;
            case 'f':
                fps = std::stoi(optarg);
                break;
            case 'w':
                width = std::stoi(optarg);
                break;
            case 'l':
                height = std::stoi(optarg);
                break;
            default:
                std::cerr << "Unknown option.\n";
                show_help = true;
        }
    }

    if (optind < argc) {
        input_file = argv[optind];
    }
}

void CLI::run()
{
    if (show_help)
    {
        print_help();
        return;
    }

    if (input_file.empty())
    {
        std::cerr << "Error: No input file\n";
        print_help();
        return;
    }

    // Run conversion + play video
    try {
        // Class init
        img::Converter converter = img::Converter(input_file, width, height);
        vid::VidPlayer video_player = vid::VidPlayer();

        // Get video
        img::VidASCII frames = converter.convert();

        // Play the video
        video_player.play_video(frames, fps);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}




