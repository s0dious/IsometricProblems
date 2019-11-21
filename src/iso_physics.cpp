#include "iso_physics.hpp"

namespace iso
{
    Collidable::Collidable(CollidableShape p_shape, CollidableType p_type):
        m_shape(p_shape),
        m_type(m_type)
    {
    }

    bool Collidable::set_properties(glm::vec3 center, GLfloat height, GLfloat width, GLfloat depth)
    {
        if(m_shape == CollidableShape::Cuboid)
        {
            return true;
        }

        return false;
    }

    bool Collidable::set_properties(glm::vec3 center, GLfloat radius, GLfloat height)
    {
        if(m_shape == CollidableShape::Cylinder)
        {
            return true;
        }

        return false;
    }

    bool Collidable::set_properties(glm::vec3 center, GLfloat radius)
    {
        if(m_shape == CollidableShape::Sphere)
        {
            return true;
        }

        return false;
    }


    PhysicsController::PhysicsController()
    {
    }


    void PhysicsController::update(std::vector<iso::Character>& p_character_list, float time_delta)
    {
        // Update each character
        for(std::vector<iso::Character>::size_type i = 0; i < p_character_list.size(); i++)
        {
            iso::Character& current_character = p_character_list[i];

            // Apply friction and air resistance

            current_character.m_acceleration = -1 * current_character.m_physics.traction * current_character.m_speed;

            // Update physics state and apply bounds

            // Update speed
            current_character.m_speed += current_character.m_acceleration * time_delta;
            std::cout << "Speed: " << current_character.m_speed.x << " " << current_character.m_speed.y << " " << current_character.m_speed.z << std::endl;

            const float max_speed = 2*current_character.m_physics.movement_speed;
            const float min_speed = 0.01;
            if(glm::length(current_character.m_speed) > max_speed)
            {
                current_character.m_speed = max_speed * glm::normalize(current_character.m_speed);
            }
            if(glm::length(current_character.m_speed) < min_speed)
            {
                current_character.m_speed = glm::vec3(0.0f, 0.0f, 0.0f);
            }
        

            // Update position
            current_character.m_position += current_character.m_speed * time_delta;

            std::cout << "Positon: " << current_character.m_position.x << " " << current_character.m_position.y << " " << current_character.m_position.z << std::endl;

            if(current_character.m_position.x < 0.0f)
            {
                current_character.m_position.x = 0.0f;
                current_character.m_speed.x = 0.0f;
            }
            else if(current_character.m_position.x > 127.0f)
            {
                current_character.m_position.x = 127.0f;
                current_character.m_speed.x = 0.0f;
            }
            if(current_character.m_position.z < 0.0f)
            {
                current_character.m_position.z = 0.0f;
                current_character.m_speed.z = 0.0f;
            }
            else if(current_character.m_position.z > 127.0f)
            {
                current_character.m_position.z = 127.0f;
                current_character.m_speed.z = 0.0f;
            }

        }
    }
}