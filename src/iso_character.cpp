#include "iso_character.hpp"

#include <iostream>

namespace iso
{    
    Character::Character(iso::CharacterModel p_character_model, glm::vec3 p_position, iso::InputType p_input_type, iso::CameraType p_camera_type):
        m_physics(p_character_model.physics),
        m_input_type(p_input_type),
        m_double_jump_count(2),
        m_position(p_position),
        m_speed(0.0f, 0.0f, 0.0f),
        m_acceleration(0.0f, 0.0f, 0.0f),
        m_front(0.0f, 0.0f, 1.0f),
        m_up(0.0f, 1.0f, 0.0f),
        m_right(1.0f, 0.0f, 0.0f),
        m_yaw(90.0f),
        m_pitch(0.0f),
        m_camera_type(p_camera_type),
        m_remaining_double_jump_count(2),
        m_animation_frame(0)
    {
        std::vector<GLfloat> data = 
        {
            -0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
            0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f,  0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f, 0.0f,  0.0f,  1.0f, 
            0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f,  0.0f,  1.0f,

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
            2, 3, 0,

            4, 5, 6,
            6, 7, 4,

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
        
        iso::Drawable drawable(material, m_position, data, indices);

        m_character_drawable = drawable;
    }


    iso::Camera Character::get_camera()
    {
        glm::vec3 camera_pos = m_position - 12.0f*m_front + glm::vec3(0.0f, 12.0f, 0.0f);

        iso::Camera camera(glm::lookAt(camera_pos, m_position + 2.0f * m_front, m_up), camera_pos);
        // std::cout << "Position from here: " << m_position.x << " " << m_position.y << " " << m_position.z << std::endl;


        return camera;
    }

    iso::Drawable Character::get_drawable()
    {
        m_character_drawable.frames[0] = iso::Transform(m_position, 0.0f, -1.0f * m_yaw, 0.0f, glm::vec3(1.1f, 2.0f, 1.1f));

        return m_character_drawable;
    }


    void CharacterController::update_input(std::vector<iso::Character>& p_character_list, const float p_time_delta) 
    {
        // Update each character based on their new input state
        for(std::vector<iso::Character>::size_type i = 0; i < p_character_list.size(); i++)
        {
            iso::Character& current_character = p_character_list[i];
        
            // Update keyboard input
            for(std::vector<iso::KeyboardInput>::size_type i = 0; i < current_character.m_keyboard_input.size(); i++)
            {
                switch(current_character.m_keyboard_input[i])
                {
                    case iso::KeyboardInput::Up:
                        current_character.m_speed += glm::normalize(glm::vec3(current_character.m_front.x, 0.0f, current_character.m_front.z)) * p_time_delta * current_character.m_physics.movement_speed;
                        break;
                    case iso::KeyboardInput::Down:
                        current_character.m_speed -= glm::normalize(glm::vec3(current_character.m_front.x, 0.0f, current_character.m_front.z)) * p_time_delta * current_character.m_physics.movement_speed;
                        break;
                    case iso::KeyboardInput::Right:
                        current_character.m_speed += glm::normalize(glm::vec3(current_character.m_right.x, 0.0f, current_character.m_right.z)) * p_time_delta * current_character.m_physics.movement_speed;
                        break;
                    case iso::KeyboardInput::Left:
                        current_character.m_speed -= glm::normalize(glm::vec3(current_character.m_right.x, 0.0f, current_character.m_right.z)) * p_time_delta * current_character.m_physics.movement_speed;
                        break;
                    case iso::KeyboardInput::Space:
                        if(current_character.m_can_jump)
                        {
                            current_character.m_speed += glm::vec3(0.0f, 20.0f, 0.0f);
                            current_character.m_can_jump = false;
                        }
                        break;
                    default:
                        break;
                }
            }
            current_character.m_keyboard_input.clear();

            // Update mouse input
            glm::vec2 mouse_input = current_character.m_mouse_input;

            current_character.m_pitch = 90.0f - mouse_input.y * (180.0f/1080.0f);
            current_character.m_yaw = 0.0f + mouse_input.x * (360.0f/1920.0f);

            if(current_character.m_pitch < -90.0f)
            {
                current_character.m_pitch = -90.0f;
            }
            else if(current_character.m_pitch > 90.0f)
            {
                current_character.m_pitch = 90.0f;
            }

            // if(current_character.m_)

            glm::vec3 t_front;
            t_front.x = cos(glm::radians(current_character.m_yaw)) * cos(glm::radians(current_character.m_pitch));
            t_front.y = sin(glm::radians(current_character.m_pitch));
            t_front.z = sin(glm::radians(current_character.m_yaw)) * cos(glm::radians(current_character.m_pitch));

            current_character.m_front = glm::normalize(t_front);

            current_character.m_right = glm::normalize(glm::cross(current_character.m_front, glm::vec3(0.0f, 1.0f, 0.0f)));
            current_character.m_up = glm::normalize(glm::cross(current_character.m_right, current_character.m_front));
        }
    }
}