#ifndef _ISO_CAMERA_HPP
#define _ISO_CAMERA_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "iso_character.hpp"

namespace iso
{
    class CameraController
    {
    public:
        void update(std::vector<iso::Character> p_character_list);
        glm::mat4 get_view(iso::Character p_character);
        glm::vec3 get_position(iso::Character p_character);
    };
};

#endif