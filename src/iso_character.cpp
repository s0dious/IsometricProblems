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
                        break;
                    default:
                        break;
                }
            }
            current_character.m_keyboard_input.clear();

            // Update mouse input
            glm::vec2 mouse_input = current_character.m_mouse_input;

            current_character.m_pitch = 90.0f - mouse_input.y * (180.0f/1080.0f);
            current_character.m_yaw = 0.0f + mouse_input.x * (360/1920.0f);

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