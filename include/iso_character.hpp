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

        iso::Camera get_camera()
        {
            iso::Camera camera(glm::lookAt(m_position, m_position + m_front, m_up), m_position);
            std::cout << "Position from here: " << m_position.x << " " << m_position.y << " " << m_position.z << std::endl;


            return camera;
        }

        iso::Drawable get_drawable()
        {
            std::vector<GLfloat> data = 
            {
                -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
                0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
                0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,

                -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,
                0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,
                0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 
                -0.5f,  0.5f,  0.5f,0.0f,  0.0f,  1.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

                0.5f,  0.5f,  0.5f, 1.0f,  0.0f,  0.0f,
                0.5f,  0.5f, -0.5f, 1.0f,  0.0f,  0.0f,
                0.5f, -0.5f, -0.5f, 1.0f,  0.0f,  0.0f, 
                0.5f, -0.5f,  0.5f, 1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f,
                0.5f, -0.5f, -0.5f, 0.0f, -1.0f,  0.0f, 
                0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f, 0.0f, -1.0f,  0.0f,

                -0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
                0.5f,  0.5f, -0.5f, 0.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, 0.0f,  1.0f,  0.0f
            };

            std::vector<GLint> indices =
            {
                0, 1, 2,
                2, 3, 1,

                4, 5, 6,
                6, 7, 5,

                8, 9, 10,
                10, 11, 8,

                12, 13, 14,
                14, 15, 12,

                16, 17, 18,
                18, 19, 16,

                20, 21, 22,
                22, 23, 20
            };

            iso::MaterialModel material(glm::vec3(1.0f, 0.0f, 0.2f), glm::vec3(1.0f, 0.2f, 0.3f), glm::vec3(0.5f, 0.5f, 0.2f), 32.0f);
            
            iso::Drawable drawable(data, indices, material);

            return drawable;
        }

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