#ifndef _ISO_INPUT_HPP
#define _ISO_INPUT_HPP

#include <vector>
#include <SFML/Window.hpp>

#include "glm/vec2.hpp"

#include "iso_character.hpp"

namespace iso
{
    class InputController
    {
    public:
        InputController(sf::Window& p_window);
        void update(std::vector<iso::Character>& p_character_list);

    private:
        sf::Window& m_window;
    };
}


#endif