#ifndef _ISO_PHYSICS_HPP
#define _ISO_PHYSICS_HPP

#include "glm/vec3.hpp"

#include "iso_database.hpp"

namespace iso
{
    class Physics
    {
    public:
        Physics(glm::vec3 p_position, glm::vec3 p_speed, glm::vec3 p_acceleration);

        void adjust_position(glm::vec3 p_position);
        void adjust_speed(glm::vec3 p_speed);
        void adjust_acceleration(glm::vec3 p_acceleration);

        void set_position(glm::vec3 p_position);
        void set_speed(glm::vec3 p_speed);
        void set_acceleration(glm::vec3 p_acceleration);

        glm::vec3 get_position();
        glm::vec3 get_speed();
        glm::vec3 get_acceleration();

    private:
        glm::vec3 m_position;
        glm::vec3 m_speed;
        glm::vec3 m_acceleration;
    };
}

#endif