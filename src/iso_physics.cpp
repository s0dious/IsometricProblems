#include "iso_physics.hpp"

namespace iso
{
    Collidable::Collidable(CollidableShape p_shape, CollidableType p_type):
        shape(p_shape),
        type(p_type)
    {
    }

    CuboidCollidable::CuboidCollidable(CollidableType p_type, glm::vec3 p_center, GLfloat p_height, GLfloat p_width, GLfloat p_depth):
        Collidable(CollidableShape::Cuboid, p_type)
    {
        m_properties.center = p_center;
        m_properties.height = p_height;
        m_properties.width = p_width;
        m_properties.depth = p_depth;
    }

    glm::vec3 CylinderCollidable::check_collision(Collidable p_collidable)
    {
        switch(p_collidable.shape)
        {
            case CollidableShape::Sphere :
                break;
            case CollidableShape::Cuboid :
                break;
            case CollidableShape::Cylinder :
                break;
            default:
                break;
        }
    }


    CylinderCollidable::CylinderCollidable(CollidableType p_type, glm::vec3 p_center, GLfloat p_radius, GLfloat p_height):
        Collidable(CollidableShape::Cylinder, p_type)
    {
        m_properties.center = p_center;
        m_properties.radius = p_radius;
        m_properties.height = p_height;
    }

    glm::vec3 CylinderCollidable::check_collision(Collidable p_collidable)
    {
        switch(p_collidable.shape)
        {
            case CollidableShape::Sphere :
                break;
            case CollidableShape::Cuboid :
                break;
            case CollidableShape::Cylinder :
                break;
            default:
                break;
        }
    }


    SphereCollidable::SphereCollidable(CollidableType p_type, glm::vec3 p_center, GLfloat p_radius):
        Collidable(CollidableShape::Sphere, p_type)
    {
        m_properties.center = p_center;
        m_properties.radius = p_radius;
    }

    glm::vec3 SphereCollidable::check_collision(Collidable p_collidable)
    {
        switch(p_collidable.shape)
        {
            case CollidableShape::Sphere :
                break;
            case CollidableShape::Cuboid :
                break;
            case CollidableShape::Cylinder :
                break;
            default:
                break;
        }
    }


    PhysicsController::PhysicsController()
    {
    }


    collidable_id_t PhysicsController::add_collidable(Collidable p_collidable)
    {
        switch(p_collidable.type)
        {
            case CollidableType::Character :
            {
                m_character.push_front(p_collidable);
                break;
            }
            case CollidableType::Environment :
            {
                m_environment.push_front(p_collidable);
                break;
            }
            case CollidableType::Projectile :
            {
                m_projectile.push_front(p_collidable);
                break;
            }
            case CollidableType::Move :
            {
                m_move.push_front(p_collidable);
                break;
            }
            default:
            {
                break;
            }
        }
    }


    collidable_id_t PhysicsController::add_collidable(Collidable p_collidable, collidable_id_t p_collidable_id)
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