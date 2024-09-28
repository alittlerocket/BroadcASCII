cmake_minimum_required(VERSION 3.10)

# Set the project name
project(OpenCV_Project)

# Find the OpenCV package
find_package(OpenCV REQUIRED)

# Add the executable
add_executable(main main.cpp)

# Link OpenCV libraries
target_link_libraries(main ${OpenCV_LIBS})
