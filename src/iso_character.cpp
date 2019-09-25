#include "iso_character.hpp"

namespace iso
{
    Character::Character(iso::Input& p_input, iso::Physics& p_physics, iso::Camera& p_camera):
        m_input(p_input),
        m_physics(p_physics),
        m_camera(p_camera) 
    { }

    void Character::update(float p_time_delta, bool p_lock_y = true)
    {
        //temp
        float m_speed = 10.0f;

        // Get input
        glm::vec3 movement_speed(0.0f, 0.0f, 0.0f);

        std::vector<iso::KeyboardInput> current_input = m_input.poll_keyboard();
        for(std::vector<iso::KeyboardInput>::size_type i = 0; i < current_input.size(); i++)
        {
            switch(current_input[i])
            {
                case iso::KeyboardInput::Up:
                    glm::vec3 front = m_camera.get_front();
                    if(p_lock_y)
                    {
                        movement_speed += glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * p_time_delta * m_speed;
                    }
                    else
                    {
                        movement_speed += front * p_time_delta * m_speed;
                    }
                    break;
                case iso::KeyboardInput::Right:
                    glm::vec3 right = m_camera.get_right();
                    if(p_lock_y)
                    {
                        movement_speed += glm::normalize(glm::vec3(right.x, 0.0f, right.z)) * p_time_delta * m_speed;
                    }
                    {
                        movement_speed += right * p_time_delta * m_speed;
                    }
                    break;
                case iso::KeyboardInput::Down:
                    glm::vec3 front = m_camera.get_front();
                    if(p_lock_y)
                    {
                        movement_speed -= glm::normalize(glm::vec3(front.x, 0.0f, front.z)) * p_time_delta * m_speed;
                    }
                    else
                    {
                        movement_speed -= front * p_time_delta * m_speed;
                    }
                    break;
                case iso::KeyboardInput::Left:

                    break;
                case iso::KeyboardInput::Space:

                    break;
                case iso::KeyboardInput::Escape:
                    m_should_close = true;
                    break;
            }
        }

        // Update Physics

        // Update Camera

    }

    bool Character::close_requested()
    {
        return m_should_close;
    }

    glm::vec3 Character::get_position()
    {

    }

    glm::mat4 Character::get_view_matrix()
    {

    }
}