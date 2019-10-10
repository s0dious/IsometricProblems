#ifndef _ISO_DATABASE_HPP
#define _ISO_DATABASE_HPP

#include <string>
#include <sqlite3.h> 

#include "glm/vec3.hpp"
#include "glad/glad.h"

namespace iso
{

    class Model
    {

    };


    class PhysicsModel : public Model
    {
    public:
        /**
         *  Direct constructor
         */
        PhysicsModel(
            float p_movement_speed, 
            float p_backwards_ratio, 
            float p_drift_acceleration, 
            float p_max_drift_speed,
            float p_gravity,
            float p_traction,
            float p_weight );

        /**
         *  Database constructor
         */
        PhysicsModel(uint p_physics_id);

        const std::string table_name = "physics";

        const float movement_speed;
        const float backwards_ratio;
        const float drift_acceleration;
        const float max_drift_speed;
        const float gravity;
        const float traction;
        const float weight;
    };


    class CharacterModel : public Model
    {
    public:
        /**
         *  Direct Constructor
         */
        CharacterModel(PhysicsModel p_physics);

        /**
         *  Database Constructor
         */
        CharacterModel(uint p_character_id);

        const std::string table_name = "character";

        const PhysicsModel physics;
        const uint double_jump_count;
    };


    class MaterialModel : public Model
    {
    public:
        /**
         *  Empty constructor
         */
        MaterialModel()
            :
            ambient(0.0f, 0.0f, 0.0f),
            diffuse(0.0f, 0.0f, 0.0f),
            specular(0.0f, 0.0f, 0.0f)
        {

        }

        /**
         *  Direct constructor
         */
        MaterialModel(
            glm::vec3 p_ambient,
            glm::vec3 p_diffuse,
            glm::vec3 p_specular,
            GLfloat p_shininess )
            :
            ambient(p_ambient),
            diffuse(p_diffuse),
            specular(p_specular),
            shininess(p_shininess)
        {

        }

        /**
         *  Database constructor
         */
        MaterialModel(uint p_material_id);

        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        GLfloat shininess;
    };


    class LightModel : public Model
    {
    public:
        /**
         *  Direct constructor
         */
        LightModel(
            glm::vec3 p_position,
            glm::vec3 p_ambient,
            glm::vec3 p_diffuse,
            glm::vec3 p_specular )
            :
            position(p_position),
            ambient(p_ambient),
            diffuse(p_diffuse),
            specular(p_specular)
        {

        }

        /**
         *  Database constructor
         */
        LightModel(uint p_light_id);

        glm::vec3 position;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    };
};

#endif