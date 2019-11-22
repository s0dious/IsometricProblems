#ifndef _ISO_PHYSICS_HPP
#define _ISO_PHYSICS_HPP

#include <vector>
#include <list>

#include "glm/vec3.hpp"

#include "iso_character.hpp"
#include "iso_database.hpp"

namespace iso
{
    typedef size_t collidable_id_t;

    enum class CollidableShape
    {
        Cuboid,
        Cylinder,
        Sphere
    };

    enum class CollidableType
    {
        Environment,
        Character,
        Projectile,
        Move
    };

    class Collidable
    {
    public:
        Collidable(CollidableShape p_shape, CollidableType p_type);
        glm::vec3 check_collision(Collidable p_collidable);

        CollidableShape shape;
        CollidableType type;

    private:
    };


    class CuboidCollidable : Collidable
    {
    public:
        CuboidCollidable(CollidableType p_type, glm::vec3 p_center, GLfloat p_height, GLfloat p_width, GLfloat p_depth);
        glm::vec3 check_collision(Collidable p_collidable);

    private:
        struct
        {
            glm::vec3 center;
            GLfloat height;
            GLfloat width;
            GLfloat depth;
        } m_properties;
    };


    class CylinderCollidable : Collidable
    {
    public:
        CylinderCollidable(CollidableType p_type, glm::vec3 p_center, GLfloat p_radius, GLfloat p_height);
        glm::vec3 check_collision(Collidable p_collidable);

    private:
        struct
        {
            glm::vec3 center;
            GLfloat radius;
            GLfloat height;
        } m_properties;
    };


    class SphereCollidable : Collidable
    {
    public:
        SphereCollidable(CollidableType p_type, glm::vec3 p_center, GLfloat p_radius);
        glm::vec3 check_collision(Collidable p_collidable);

    private:
        struct
        {
            glm::vec3 center;
            GLfloat radius;
        } m_properties;
    };

    class PhysicsController
    {
    public:
        PhysicsController();
        void update(std::vector<iso::Character>& p_character_list, float time_delta);

        collidable_id_t add_collidable(Collidable p_collidable);
        collidable_id_t add_collidable(Collidable p_collidable, collidable_id_t p_collidable_id);
        
    private:
        std::list<Collidable> m_environment;
        std::list<Collidable> m_character;
        std::list<Collidable> m_move;
        std::list<Collidable> m_projectile;
    };
}

#endif