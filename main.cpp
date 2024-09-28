#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Load an image from file
    cv::Mat image = cv::imread("image.jpg");

    if (image.empty()) {
        std::cerr << "Could not read the image" << std::endl;
        return 1;
    }

    // Display the image
    cv::imshow("Display window", image);

    // Wait for a key press
    cv::waitKey(0);

    return 0;
}
