#ifndef _ISO_PHYSICS_HPP
#define _ISO_PHYSICS_HPP

#include <vector>

#include "glm/vec3.hpp"

#include "iso_character.hpp"
#include "iso_database.hpp"

namespace iso
{
    class PhysicsController
    {
        void update(std::vector<iso::Character>& p_character_list);
    };
}

#endif