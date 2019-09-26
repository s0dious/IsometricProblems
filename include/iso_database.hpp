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

    private:
        const std::string c_table_name = "physics";

        const float m_movement_speed;
        const float m_backwards_ratio;
        const float m_drift_acceleration;
        const float m_max_drift_speed;
        const float m_gravity;
        const float m_traction;
        const float m_weight;
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
    private:
        const std::string c_table_name = "character";

        const PhysicsModel m_physics;
        const uint m_double_jump_count;
    };
};

#endif