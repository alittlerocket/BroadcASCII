#include "test/testcases.hpp"

int main() {
    try
    {
        //render_simple("test_images/4colors.png");
        test_ascii_conversion("test_images/100x100_amogus.png");
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}