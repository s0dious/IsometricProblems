#ifndef _ISO_INPUT_HPP
#define _ISO_INPUT_HPP

#include <vector>
#include <SFML/Window.hpp>

#include "glm/vec2.hpp"

namespace iso
{
    typedef glm::vec2 MouseInput;

    enum class KeyboardInput
    {
        Left,
        Right,
        Up,
        Down,
        Space,
        Escape
    };

    class Input
    {
    public:
        virtual std::vector<KeyboardInput> poll_keyboard() = 0;
        virtual MouseInput poll_mouse() = 0;
    };


    class LocalInput : public Input
    {
    public:
        LocalInput(sf::Window& p_window);
        std::vector<KeyboardInput> poll_keyboard();
        MouseInput poll_mouse();

    private:
        sf::Window& m_window;
    };
}


#endif