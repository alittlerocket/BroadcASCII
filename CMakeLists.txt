#include(FetchContent)
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(BroadcASCII)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directory (update this to match your actual path)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the testing executable
add_executable(broadcascii 
    src/CLI/CLI.cpp
    src/img/Converter.cpp
    src/vid/VidPlayer.cpp
)
