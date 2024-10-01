#include "test/testcases.hpp"

#include <exception>
#include <iostream>

int main() {
    try
    {
        render_simple();
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}