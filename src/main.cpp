
// C/C++ Libraries
#include <iostream>

// SFML Libraries
#include <SFML/Window.hpp>

// OpenGL Libraries
#include <glad/glad.h> 

int main()
{
    // Set some OpenGL context settings
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2; // Optional

    // Request OpenGL version 3.3
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    // Create OpenGL context
    sf::Window window(sf::VideoMode(800, 800), "OpenGL", sf::Style::Default, settings);

    // GLAD will find the proper opengl functions at runtime for cross platform compatability
    gladLoadGL();

    // Event loop where all the magic happens
    while(window.isOpen()) {
        sf::Event event;

        while(window.pollEvent(event)) {

            // Process events
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

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Push buffer to screen
        window.display();
    }

    return 0;
}