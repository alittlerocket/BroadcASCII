include(FetchContent)

cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(BroadcASCII)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(PkgConfig REQUIRED)
pkg_check_modules(AVFORMAT REQUIRED libavformat)
pkg_check_modules(AVCODEC REQUIRED libavcodec)
pkg_check_modules(AVUTIL REQUIRED libavutil)
pkg_check_modules(SWSCALE REQUIRED libswscale)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
	include
    ${AVFORMAT_INCLUDE_DIRS}
    ${AVCODEC_INCLUDE_DIRS}
    ${AVUTIL_INCLUDE_DIRS}
    ${SWSCALE_INCLUDE_DIRS}
)

# Add the main executable
add_executable(main src/main.cpp)

# Link against X11 and other necessary libraries (for image display functionality)
target_link_libraries(
    main 
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
)

# Add the testing executable
add_executable(test_main 
    src/test_main.cpp
    src/img/ImgConverter.cpp
)

# Link this too.
target_link_libraries(
    test_main
    ${AVFORMAT_LIBRARIES}
    ${AVCODEC_LIBRARIES}
    ${AVUTIL_LIBRARIES}
    ${SWSCALE_LIBRARIES}
)

