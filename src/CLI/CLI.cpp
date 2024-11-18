#include <exception>
#include <getopt.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <CLI/CLI.hpp>

CLI::CLI(const int argc, char* argv[])
{
    set_default_dim();
    set_default_fps();
    parse_arguments(argc, argv);
}

void CLI::print_help()
{
    std::cout << 
        "Usage: broadcascii [OPTION...] <input-video>\n" <<
        "Converts the input video to ascii art with ANSI color codes\n" <<
        "\t-h, --help                       display this help and exit\n" <<
        "\t-h, --fps                        sets the target_fps to <fps. Default is fps of input_video>\n" <<
        "\t-w, --width <target_width>       sets the target_width to <target_width>. Default is current terminal width\n" <<
        "\t-h, --height <target_height>     sets the target_height to <target_height>. Default is current terminal height\n" <<
    std::endl;
}

void CLI::set_default_dim()
{
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
        std::cerr << "ioctl" << std::endl;
        width = 80;
        height = 24;
    } else {
        width = w.ws_col;
        height = w.ws_row;
    }
}

void CLI::set_default_fps()
{
    std::string probe_cmd = "ffprobe -v error -select_streams v:0 -show_entries stream=r_frame_rate -of default=noprint_wrappers=1:nokey=1 " + input_file;
    FILE* probe_pipe = popen(probe_cmd.c_str(), "r");
    if (!probe_pipe) 
    {
        throw img::ConversionException("Failed to open probe pipe!");
    }
    
    char fps_fraction[50];
    if (fscanf(probe_pipe, "%49s", fps_fraction) != 1) {
        pclose(probe_pipe);
        throw img::ConversionException("Failed to read frame rate from probe pipe!");
    }
    pclose(probe_pipe);

    try {
        std::string fps_str(fps_fraction);
        int pos = fps_str.find('/');
        if (pos != std::string::npos) 
        {
            int num, denom = std::stoi(fps_str.substr(0, pos)), denominator = std::stoi(fps_str.substr(pos + 1));
            fps = static_cast<double>(num) / denom;
        } else 
        {
            fps = std::stod(fps_str);
        }
    } catch (const std::exception& e) {
        throw img::ConversionException("Failed to parse frame rate: " + std::string(e.what()));
    }

    std::cout << "Default FPS: " << fps << std::endl;
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
        img::Converter converter = img::Converter(input_file, fps, width, height);
        vid::VidPlayer video_player = vid::VidPlayer();

        // Get video
        img::VidASCII frames = converter.convert();

        // Start audio playback in a separate thread
        std::thread audio_thread(vid::play_audio, input_file);

        // Play the video
        video_player.play_video(frames, fps);

        // Wait for the audio thread to finish
        audio_thread.join();

    
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
}




