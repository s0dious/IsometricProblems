#ifndef _ISO_CHARACTER_HPP
#define _ISO_CHARACTER_HPP

#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"

#include "iso_input.hpp"
#include "iso_database.hpp"

namespace iso
{
    // class Character
    // {
    // public:
    //     Character(iso::Input& p_input, iso::Physics& p_physics, iso::Camera& p_camera);
    //     void update(float p_time_delta, bool p_lock_y = true);
    //     bool close_requested();

    //     glm::vec3 get_position();
    //     glm::mat4 get_view_matrix();

    // private:
    //     iso::Input& m_input;
    //     iso::Physics& m_physics;
    //     iso::Camera& m_camera;

    //     bool m_should_close;
    // };

    class Character
    {
    public:
        Character(iso::CharacterModel p_character_model, glm::vec3 p_position, iso::InputType p_input_type);
    private:
        // Character attributes
        iso::PhysicsModel m_physics;
        iso::InputType m_input_type;
        uint m_double_jump_count;

        // Character state
        glm::vec3 m_position;
        glm::vec3 m_speed;
        glm::vec3 m_acceleration;

        uint m_remaining_double_jump_count;
        uint m_animation_frame;
    };
}

#endif