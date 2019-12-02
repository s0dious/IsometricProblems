#ifndef _ISO_PHYSICS_HPP
#define _ISO_PHYSICS_HPP

#include <vector>
#include <list>

#include "glm/vec3.hpp"

#include "iso_character.hpp"
#include "iso_database.hpp"

#define HALT_X 1
#define HALT_Y 2
#define HALT_Z 4

namespace iso
{
    typedef size_t collidable_id_t;
    typedef uint8_t physics_result_t;

    struct WorldCollidable
    {
    public:
        WorldCollidable(glm::vec3 p_center, GLfloat p_width, GLfloat p_height, GLfloat p_depth);
        bool test_point(glm::vec3 p_point);

        glm::vec3 center; 
        GLfloat width; 
        GLfloat height; 
        GLfloat depth;
    };


    struct CharacterCollidable
    {
    public:
        CharacterCollidable(glm::vec3 p_center, GLfloat p_height, GLfloat p_radius);
        bool test_point(glm::vec3 p_point);

        glm::vec3 center;
        GLfloat height;
        GLfloat radius;
    };


    class PhysicsController
    {
    public:
        // PhysicsController();
        void update(std::vector<iso::Character>& p_character_list, float time_delta);

        collidable_id_t add_collidable(const WorldCollidable& p_collidable);
        collidable_id_t update_collidable(const WorldCollidable& p_collidable, collidable_id_t p_collidable_id);

        std::pair<glm::vec3, physics_result_t> test_collision(const CharacterCollidable& p_character, const WorldCollidable& p_world);
        
    private:
        std::vector<WorldCollidable> m_environment;
    };
}

#endif