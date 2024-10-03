#include "test/testcases.hpp"
#include "img/ImgConverter.hpp"
#include "img/RGBConverter.hpp"
#include <cstdlib>


int render_simple(std::string path) 
{
    // Load an image to display
    sf::Texture texture;
    if (!texture.loadFromFile(path))
    {
        std::cerr << "Could not load image" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Sprite sprite(texture);

    // Create window
    int spriteWidth = texture.getSize().x;
    int spriteHeight = texture.getSize().y;
    sf::RenderWindow window(sf::VideoMode(spriteWidth, spriteHeight), 
                            "SFML window", 
                            sf::Style::Titlebar);

    // Start the loop
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Clear screen
        window.clear();

        // Draw the sprite
        window.draw(sprite);

        // Update window
        window.display();
    }

    return EXIT_SUCCESS;
}

int test_ascii_conversion(std::string path)
{
    // Get the image converter
    img::ImageConverter i = img::ImageConverter(path);

    // Load the rgb matrix.
    i.load();

    // Place it in a vector
    std::vector<img::RGBMatrix> rgbMatrices = { i.get_rgbMatrix() };

    // Initialize the rgb converter
    img::RGBConverter r = img::RGBConverter(rgbMatrices);

    // Render the matrices
    r.render_ascii();

    // Print out what it look like
    for ( const img::ImgASCII& mat : r.get_ascii() )
    {
        std::cout << "Image of " << path << std::endl;
        for ( std::string s : mat)
        {
            for (char c : s)
                std::cout << c << ' ';
            std::cout << std::endl;
        }
        
    }

    return EXIT_SUCCESS;
}