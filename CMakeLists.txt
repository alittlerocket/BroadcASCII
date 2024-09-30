include(FetchContent)

cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(BroadcASCII)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Don't build SFML's audio or network libs
set(SFML_BUILD_AUDIO FALSE CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK FALSE CACHE BOOL "" FORCE)

FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.1)

FetchContent_MakeAvailable(SFML)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
	include
	${SFML_SOURCE_DIR}/include)

# Add the main executable
add_executable(main src/main.cpp)

# Link against X11 and other necessary libraries (for image display functionality)
target_link_libraries(main sfml-graphics)

# Add the testing executable
add_executable(test src/test.cpp)

# Link this too.
target_link_libraries(test sfml-graphics)

