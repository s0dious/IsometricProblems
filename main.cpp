#include <iostream>

#include <SFML/Window.hpp>

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2; // Optional

    // Request OpenGL version 3.2
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Close, settings);

    while(window.isOpen()) {
        sf::Event event;

        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            else if(event.type == sf::Event::KeyPressed) {
                std::cout << "Key pressed" << std::endl;

                if(event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }
    }

    return 0;
}