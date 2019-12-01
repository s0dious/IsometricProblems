#include "iso_physics.hpp"

namespace iso
{
    WorldCollidable::WorldCollidable(glm::vec3 p_center, GLfloat p_height, GLfloat p_width, GLfloat p_depth):
        center(p_center),
        width(p_width),
        height(p_height),
        depth(p_depth)
    {

    }


    CharacterCollidable::CharacterCollidable(glm::vec3 p_center, GLfloat p_height, GLfloat p_radius):
        center(p_center),
        height(p_height),
        radius(p_radius)
    {

    }


    collidable_id_t PhysicsController::add_collidable(const WorldCollidable& p_collidable)
    {
        m_environment.push_back(p_collidable);
        return m_environment.size()-1;
    }


    collidable_id_t PhysicsController::update_collidable(const WorldCollidable& p_collidable, collidable_id_t p_collidable_id)
    {
        m_environment[p_collidable_id] = p_collidable;
        return p_collidable_id;
    }


    collidable_id_t PhysicsController::add_collidable(const CharacterCollidable& p_collidable)
    {
        m_character.push_back(p_collidable);
        return m_character.size()-1;
    }


    collidable_id_t PhysicsController::update_collidable(const CharacterCollidable& p_collidable, collidable_id_t p_collidable_id)
    {
        m_character[p_collidable_id] = p_collidable;
        return p_collidable_id;
    }


    glm::vec3 PhysicsController::test_collision(const CharacterCollidable& p_character, const WorldCollidable& p_world)
    {
        // Check if the Z axis overlaps
        if(p_character.center.y - p_character.height/2 <= p_world.center.y + p_world.height/2
            && p_character.center.y + p_character.height/2 >= p_world.center.y - p_world.height/2
        )
        {
            // Check if the horizontal cross sections overlap
        }

        return glm::vec3(0.0f, 0.0f, 0.0f);
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