# BroadcASCII

This project converts video frames into colored ASCII art and displays them in the terminal. Using FFmpeg for video processing, it captures each frame, converts it to ASCII with RGB colors, and then prints it in the terminal.

## Features

- Converts video frames to ASCII characters with color based on RGB values.
- Adjusts ASCII output dynamically to match the video’s aspect ratio.
- Stores each frame as a grid of ASCII characters, allowing for sequential playback.
- Multi-threaded

## Prerequisites

- **FFmpeg**: Make sure FFmpeg is installed and accessible in your system’s PATH.
- **C++ Compiler**: A C++17-compatible compiler is recommended.

## Installation

1. Clone the repository:
   ```bash
   git clone https://github.com/alittlerocket/BroadcASCII.git
   cd video-to-ascii
   ```

2. Build with CMake:
   ```cmake
    mkdir build
    cd build
    cmake ..
    make
   ```

### Example Command

The program uses `ffmpeg` through `popen` to dynamically scale the video frames and output them as RGB data:

```cpp
std::string command = "ffmpeg -i input.mp4 -vf \"scale=100:50,format=rgb24\" -f rawvideo -";
```

### Dynamic Scaling

The program adjusts the ASCII output to match the video’s aspect ratio, ensuring a visually accurate representation of each frame.

### Color Output in Terminal

This project uses ANSI escape codes to render ASCII art in full RGB color. Ensure your terminal supports 24-bit color.

## Troubleshooting

- **FFmpeg Not Found**: Ensure FFmpeg is installed and available in your system’s PATH.
- **Terminal Color Support**: For best results, use a terminal that supports true 24-bit color.

## License

TBD
