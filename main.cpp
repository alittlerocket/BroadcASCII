#include <SFML/Graphics.hpp>
#include <iostream>

int main() {
    // Create a texture to hold the image
    sf::Texture texture;

    // Load the image into the texture
    if (!texture.loadFromFile("image.png")) 
    {
        std::cerr << "Could not load the image" << std::endl;
        return -1;
    }

    // Create a sprite to display the texture
    sf::Sprite sprite(texture);

    // Create a window to display the image
    sf::RenderWindow window(sf::VideoMode(texture.getSize().x, texture.getSize().y), "Image Display", sf::Style::Titlebar);

    // Main loop to display the image
    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
