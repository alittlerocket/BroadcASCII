#include "test/testcases.hpp"

#include "SFML/Graphics.hpp"
#include <iostream>

int render_simple() 
{
        // Load an image to display
    sf::Texture texture;
    if (!texture.loadFromFile("image.png"))
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