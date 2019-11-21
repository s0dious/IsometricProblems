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
        bool set_properties(glm::vec3 center, GLfloat height, GLfloat width, GLfloat depth);
        bool set_properties(glm::vec3 center, GLfloat radius, GLfloat height);
        bool set_properties(glm::vec3 center, GLfloat radius);


    private:
        CollidableShape m_shape;
        CollidableType m_type;
        union m_properties 
        {
            struct
            {
                glm::vec3 center;
                GLfloat height;
                GLfloat width;
                GLfloat depth;
            } cuboid;

            struct
            {
                glm::vec3 center;
                GLfloat radius;
                GLfloat height;
            } cylinder;

            struct 
            {
                glm::vec3 center;
                GLfloat radius;
            } sphere;
        };
    };


    class SphereCollidable : Collidable
    {

    };

    class CylinderCollidable : Collidable
    {

    };

    class CuboidCollidable : Collidable
    {

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