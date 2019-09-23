#ifndef _ISO_INPUT_HPP
#define _ISO_INPUT_HPP

#include <vector>

#include "glm/vec2.hpp"

namespace iso
{
    typedef glm::vec2 MouseInput;

    enum class KeyboardInput
    {
        LEFT,
        RIGHT,
        FORWARD,
        BACKWARD,

    };

    class Input
    {
    public:
        Input();
        std::vector<KeyboardInput> poll_keyboard();
        MouseInput poll_mouse();
    };

} // namespace iso


#endif