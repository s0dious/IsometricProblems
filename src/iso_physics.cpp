#include "iso_physics.hpp"

namespace iso
{
    WorldCollidable::WorldCollidable(glm::vec3 p_center, GLfloat p_width, GLfloat p_height, GLfloat p_depth):
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


    std::pair<glm::vec3, physics_result_t> PhysicsController::test_collision(const CharacterCollidable& p_character, const WorldCollidable& p_world)
    {
        glm::vec3 delta = glm::vec3(0.0f);
        physics_result_t result = 0;

        // Check if the Z axis overlaps
        if(p_character.center.y - p_character.height/2 <= p_world.center.y + p_world.height/2
            && p_character.center.y + p_character.height/2 >= p_world.center.y - p_world.height/2
        )
        {
            std::cout << "Vertical allignement found" << std::endl;
            // Check if the horizontal cross sections overlap
            glm::vec3 diff = p_character.center - p_world.center;
            glm::vec3 clamped = diff;

            std::cout << "Difference: " << diff.x << " " << diff.y << " " << diff.z << std::endl;

            // clamp X direction
            if(diff.x < -1*p_world.width/2)
            {
                clamped.x = -1*p_world.width/2;
            }
            else if(diff.x > p_world.width/2)
            {
                clamped.x = p_world.width/2;
            }

            // clamp y direction
            if(diff.y < -1*p_world.height/2)
            {
                clamped.y = -1*p_world.height/2;
            }
            else if(diff.y > p_world.height/2)
            {
                clamped.y = p_world.height/2;
            }

            // clamp z direction
            if(diff.z < -1*p_world.depth/2)
            {
                clamped.z = -1*p_world.depth/2;
            }
            else if(diff.z > p_world.depth/2)
            {
                clamped.z = p_world.depth/2;
            }

            glm::vec3 closest = p_world.center + clamped;
            diff = p_character.center - closest;

            std::cout << "Closest point: " << closest.x << " " << closest.y << " " << closest.z << std::endl;
            std::cout << "Distance: " << glm::length(glm::vec2(diff.x, diff.z)) << std::endl;
            std::cout << "Radius: " << p_character.radius << std::endl;

            // h_diff is now the closest point on the world to the character
            if(glm::length(glm::vec2(diff.x, diff.z)) < p_character.radius)
            {
                // if we get here, there's a collision
                
                // Check if we should move the character up or down

                // If the character is near the top
                if(glm::abs( (p_character.center.y - p_character.height/2) - (p_world.center.y + p_world.height/2) ) < p_world.height/5.0f )
                {
                    result |= HALT_Y;
                    delta += glm::vec3(0.0f, (p_world.center.y + p_world.height/2) - (p_character.center.y - p_character.height/2), 0.0f);
                }
                // If the character is near the bottom
                else if(glm::abs( (p_character.center.y + p_character.height/2) - (p_world.center.y - p_world.height/2) ) < p_world.height/5.0f )
                {
                    result |= HALT_Y;
                    delta += glm::vec3(0.0f, (p_world.center.y - p_world.height/2) - (p_character.center.y + p_character.height/2), 0.0f);
                }
                // otherwise we're moving horizontally
                else
                {
                    GLfloat delta_magnitude = p_character.radius - glm::length(glm::vec2(diff.x, diff.z));
                    std::cout << "Magnitude: " << delta_magnitude << std::endl;

                    delta = delta_magnitude * glm::normalize(diff);
                    // if it's moved back to one of the X sides
                    if(glm::abs(diff.x) > glm::abs(diff.z))
                    {
                        result |= HALT_X;
                    }
                    // if it's moved back to one of the Z sides
                    else if(glm::abs(diff.z) > glm::abs(diff.x))
                    {
                        result |= HALT_Z;
                    }
                    // if it's moved back to a corner
                    else
                    {
                        result |= HALT_X;
                        result |= HALT_Z;
                    }
                }
                
            }
        }

        return std::make_pair(delta, result);
    }


    void PhysicsController::update(std::vector<iso::Character>& p_character_list, float time_delta)
    {
        // Update each character
        for(std::vector<iso::Character>::size_type i = 0; i < p_character_list.size(); i++)
        {
            iso::Character& current_character = p_character_list[i];

            // Apply friction and gravity
            current_character.m_acceleration = -1 * current_character.m_physics.traction * current_character.m_speed;
            current_character.m_acceleration += glm::vec3(0.0f, -20.0f, 0.0f);

            // Update physics state and apply bounds

            // Update speed
            current_character.m_speed += current_character.m_acceleration * time_delta;
            // std::cout << "Speed: " << current_character.m_speed.x << " " << current_character.m_speed.y << " " << current_character.m_speed.z << std::endl;

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

            // std::cout << "Positon: " << current_character.m_position.x << " " << current_character.m_position.y << " " << current_character.m_position.z << std::endl;

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

            // Check collisions
            for(size_t j = 0; j < m_environment.size(); j++)
            {
                std::cout << "Testing collision between character #" << i << " and environment #" << j << std::endl;

                WorldCollidable& current_world = m_environment[j];
                CharacterCollidable current_collidable = CharacterCollidable(current_character.m_position, 1.0f, 0.5f);

                std::cout << "Environment position: " << current_world.center.x << " " << current_world.center.y << " " << current_world.center.z << std::endl;
                std::cout << "Environment height: " << current_world.center.y + current_world.height/2 << std::endl;

                std::cout << "Character position: " << current_collidable.center.x << " " << current_collidable.center.y << " " << current_collidable.center.z << std::endl;
                std::cout << "Character bottom: " << current_collidable.center.y - current_collidable.height/2 << std::endl;

                std::pair<glm::vec3, physics_result_t> collision = test_collision(current_collidable, current_world);

                std::cout << "Delta: " << collision.first.x << " " << collision.first.y << " " << collision.first.z << std::endl;
                std::cout << "Result: " << collision.second << std::endl;

                // If there's a collision
                if(collision.second)
                {
                    std::cout << "COLLISION" << std::endl;

                    // Update position
                    current_character.m_position += collision.first;

                    // Update speed and acceleration
                    if(HALT_X & collision.second)
                    {
                        current_character.m_speed.x = 0.0f;
                        current_character.m_acceleration.x = 0.0f;
                    }
                    if(HALT_Y & collision.second)
                    {
                        current_character.m_speed.y = 0.0f;
                        current_character.m_acceleration.y = 0.0f;
                        current_character.m_can_jump = true;
                    }  
                    if(HALT_Z & collision.second)
                    {
                        current_character.m_speed.z = 0.0f;
                        current_character.m_acceleration.z = 0.0f;
                    }         
                }
            }
        }
    }
}