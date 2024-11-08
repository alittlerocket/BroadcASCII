#include <CLI/CLI.hpp> // Include your CLI header

int main(int argc, char* argv[]) {
    try {
        CLI cli(argc, argv);
        cli.run();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
