#ifndef _ISO_CHARACTER_HPP
#define _ISO_CHARACTER_HPP

#include <vector>

#include "glad/glad.h"
#include <glm/glm.hpp>
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include <glm/gtc/matrix_transform.hpp>

#include "iso_camera.hpp"
#include "iso_database.hpp"

namespace iso
{
    enum class CameraType
    {
        FirstPerson,
        ThirdPerson,
        None
    };

    enum class InputType
    {
        Keyboard,
        Network
    };

    enum class KeyboardInput
    {
        Left,
        Right,
        Up,
        Down,
        Space,
        Escape
    };


    class Character
    {
    friend class CharacterController;
    friend class InputController;
    friend class PhysicsController;
    friend class CameraController;
    public:
        Character(iso::CharacterModel p_character_model, 
                    glm::vec3 p_position, 
                    iso::InputType p_input_type = iso::InputType::Keyboard, 
                    iso::CameraType p_camera_type = iso::CameraType::ThirdPerson);

        iso::Camera get_camera();

        iso::Drawable get_drawable();

    private:
        // Character attributes
        iso::PhysicsModel m_physics;
        iso::InputType m_input_type;
        uint m_double_jump_count;

        // Character state
        glm::vec3 m_position;
        glm::vec3 m_speed;
        glm::vec3 m_acceleration;

        std::vector<iso::KeyboardInput> m_keyboard_input;
        glm::vec2 m_mouse_input;

        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;

        GLfloat m_yaw;
        GLfloat m_pitch;

        iso::Drawable m_character_drawable;
        iso::CameraType m_camera_type;

        uint m_remaining_double_jump_count;
        uint m_animation_frame;
    };


    class CharacterController
    {
    public:
        void update_input(std::vector<iso::Character>& p_character_list, const float p_time_delta);
        void update_collision(std::vector<iso::Character>& p_character_list);
    private:
    };
}

#endif