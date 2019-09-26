#include "iso_input.hpp"

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
    void InputController::update(std::vector<iso::Character> p_character_list)
    {
        // Update each character
        for(std::vector<iso::Character>::size_type i = 0; i < p_character_list.size(); i++)
        {
            iso::Character& current_character = p_character_list[i];

            
        }
    }


    // Legacy code

    LocalInput::LocalInput(sf::Window& p_window):
        m_window(p_window) 
    { }

    std::vector<KeyboardInput> LocalInput::poll_keyboard()
    {
        std::vector<KeyboardInput> keyboard_input;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            keyboard_input.push_back(KeyboardInput::Up);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            keyboard_input.push_back(KeyboardInput::Right);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            keyboard_input.push_back(KeyboardInput::Left);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            keyboard_input.push_back(KeyboardInput::Down);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            keyboard_input.push_back(KeyboardInput::Space);
        }

        sf::Event event;
        while(m_window.pollEvent(event)) 
        {

            // Process events
            if(event.type == sf::Event::Closed) {
                keyboard_input.push_back(KeyboardInput::Escape);
            }
            else if(event.type == sf::Event::KeyPressed) {
                switch(event.key.code)
                {
                    case sf::Keyboard::Escape :
                        keyboard_input.push_back(KeyboardInput::Escape);
                        break;
                }
            }
        }

        return keyboard_input;
    }

    MouseInput LocalInput::poll_mouse()
    {
        sf::Vector2i mouse_input = sf::Mouse::getPosition(m_window);

        return MouseInput((float)mouse_input.x, (float)mouse_input.y);
    }
}