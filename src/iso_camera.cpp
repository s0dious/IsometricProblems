#include <iso_camera.hpp>
#include <iostream>

iso::Camera::Camera(
    glm::vec3 p_position, 
    glm::vec3 p_front, 
    glm::vec3 p_up, 
    GLfloat p_yaw, GLfloat p_pitch, 
    float p_speed, float p_sensitivity,
    float p_origin_mouse_x, float p_origin_mouse_y,
    float p_max_mouse_x, float p_max_mouse_y)
{
    m_position = p_position;
    m_front = p_front;
    m_global_up = p_up;

    m_yaw = p_yaw;
    m_pitch = p_pitch;
    m_zoom = 45.0f;

    m_sensitivity = p_sensitivity;
    m_speed = p_speed;

    m_origin_mouse_x = p_origin_mouse_x;
    m_origin_mouse_y = p_origin_mouse_y;

    update();
}

void iso::Camera::set_mouse(float p_mouse_x, float p_mouse_y)
{
    std::cout << "mouse change: " << m_origin_mouse_x - p_mouse_x << " " << m_origin_mouse_y - p_mouse_y << std::endl;

    float x_delta = m_sensitivity * -1 * (m_origin_mouse_x - p_mouse_x);
    float y_delta = m_sensitivity * -1 * (m_origin_mouse_y - p_mouse_y);

    // if(p_mouse_x < 0)
    // {
    //     float x_delta = m_sensitivity * -1 * 
    // }

    std::cout << "x y delta: "  << "<" << x_delta << ", " << y_delta << ">" << std::endl;

    m_yaw = x_delta;
    m_pitch = y_delta;

    std::cout << "yaw pitch: " << m_yaw << " " << m_pitch << std::endl;

    if(m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }
    if(m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }

    update();
}

void iso::Camera::forward(float p_time_delta, GLboolean p_lock_y)
{
    if(p_lock_y)
    {
        m_position += glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z)) * p_time_delta * m_speed;
    }
    else
    {
        m_position += m_front * p_time_delta * m_speed;
    }
}

void iso::Camera::backward(float p_time_delta, GLboolean p_lock_y)
{
    if(p_lock_y)
    {
        m_position -= glm::normalize(glm::vec3(m_front.x, 0.0f, m_front.z)) * p_time_delta * m_speed;
    }
    else
    {
        m_position -= m_front * p_time_delta * m_speed; 
    }
}

void iso::Camera::right(float p_time_delta, GLboolean p_lock_y)
{
    if(p_lock_y)
    {
        m_position += glm::normalize(glm::vec3(m_right.x, 0.0f, m_right.z)) * p_time_delta * m_speed;
    }
    {
        m_position += m_right * p_time_delta * m_speed;
    }
}

void iso::Camera::left(float p_time_delta, GLboolean p_lock_y)
{
    if(p_lock_y)
    {
        m_position -= glm::normalize(glm::vec3(m_right.x, 0.0f, m_right.z)) * p_time_delta * m_speed;
    }
    {
        m_position -= m_right * p_time_delta * m_speed;
    }
}

glm::mat4 iso::Camera::get_view()
{
    return glm::lookAt(m_position, m_position + m_front, m_up);
}

void iso::Camera::update()
{
    glm::vec3 t_front;
    t_front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    t_front.y = sin(glm::radians(m_pitch));
    t_front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_front = glm::normalize(t_front);

    m_right = glm::normalize(glm::cross(m_front, m_global_up));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

void iso::Camera::set_position(glm::vec3 p_position)
{
    m_position = p_position;
}