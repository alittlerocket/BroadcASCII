#include "test/testcases.hpp"

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
    img::ImgASCII img_ascii = img::to_ASCII(path);

    // Print out what it look like
    for ( const std::string& s : img_ascii )
    {
        for (char c : s)
            std::cout << c << ' ';
        std::cout << std::endl;
    }

    return EXIT_SUCCESS;
}