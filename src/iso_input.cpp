#include "iso_input.hpp"

#include <iostream>

namespace iso
{
    /**
     * 
     */
    InputController::InputController(sf::Window& p_window):
        m_window(p_window)
    { 

    }


    /**
     * 
     */
    void InputController::update(std::vector<iso::Character>& p_character_list)
    {
        sf::Event event;
        while(m_window.pollEvent(event)) 
        {
            // Process events
            if(event.type == sf::Event::Closed) {
                m_window.close();
            }
            else if(event.type == sf::Event::KeyPressed) {
                switch(event.key.code)
                {
                    case sf::Keyboard::Escape :
                        m_window.close();
                        break;
                    default:
                        break;
                }
            }
        }

        // Update each character
        for(std::vector<iso::Character>::size_type i = 0; i < p_character_list.size(); i++)
        {
            iso::Character& current_character = p_character_list[i];

            sf::Vector2i mouse_input = sf::Mouse::getPosition(m_window);

            // Capture mouse input on window
            if(mouse_input.x < 1)
            {
                mouse_input.x = 1918;
            }
            if(mouse_input.x > 1918)
            {
                mouse_input.x = 1;
            }
            if(mouse_input.y < 0)
            {
                mouse_input.y = 0;
            }
            if(mouse_input.y > 1080)
            {
                mouse_input.y = 1080;
            }
            sf::Mouse::setPosition(mouse_input, m_window);

            current_character.m_mouse_input = glm::vec2((float)mouse_input.x, (float)mouse_input.y);

            switch(current_character.m_input_type)
            {
                case iso::InputType::Keyboard:

                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                    {
                        current_character.m_keyboard_input.push_back(KeyboardInput::Up);
                        std::cout << "up" << std::endl;
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                    {
                        current_character.m_keyboard_input.push_back(KeyboardInput::Right);
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                    {
                        current_character.m_keyboard_input.push_back(KeyboardInput::Left);
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                    {
                        current_character.m_keyboard_input.push_back(KeyboardInput::Down);
                    }
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
                    {
                        current_character.m_keyboard_input.push_back(KeyboardInput::Space);
                    }

                    break;
                default:
                    break;
            }
        }
    }
}