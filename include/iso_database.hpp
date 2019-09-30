#ifndef _ISO_DATABASE_HPP
#define _ISO_DATABASE_HPP

#include <string>
#include <sqlite3.h> 

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
};

#endif