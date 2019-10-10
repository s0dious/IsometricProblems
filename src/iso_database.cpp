#include <iso_database.hpp>

namespace iso
{
    PhysicsModel::PhysicsModel(
            float p_movement_speed, 
            float p_backwards_ratio, 
            float p_drift_acceleration, 
            float p_max_drift_speed,
            float p_gravity,
            float p_traction,
            float p_weight ):
        movement_speed(p_movement_speed),
        backwards_ratio(p_backwards_ratio), 
        drift_acceleration(p_drift_acceleration), 
        max_drift_speed(p_max_drift_speed),
        gravity(p_gravity),
        traction(p_traction),
        weight(p_weight)
    {

    }


    CharacterModel::CharacterModel(PhysicsModel m_physics):
        physics(m_physics),
        double_jump_count(2)
    {

    }
}