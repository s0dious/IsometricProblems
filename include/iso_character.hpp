#ifndef _ISO_CHARACTER_HPP
#define _ISO_CHARACTER_HPP

#include "iso_input.hpp"
#include "iso_physics.hpp"
#include "iso_camera.hpp"

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

namespace iso
{
    class Character
    {
    public:
        Character(iso::Input& p_input, iso::Physics& p_physics, iso::Camera& p_camera);
        void update(float p_time_delta, bool p_lock_y = true);
        bool close_requested();

        glm::vec3 get_position();
        glm::mat4 get_view_matrix();

    private:
        iso::Input& m_input;
        iso::Physics& m_physics;
        iso::Camera& m_camera;

        bool m_should_close;
    };

    class Character
    {
    public:
        Character()
    private:
    };
}

#endif