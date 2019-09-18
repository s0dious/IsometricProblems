#ifndef _ISO_CAMERA_HPP
#define _ISO_CAMERA_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace iso
{
    class Camera
    {
    public:
        Camera(
            glm::vec3 p_position = glm::vec3(0.0f, 0.0f, 0.0f), 
            glm::vec3 p_front = glm::vec3(0.0f, 0.0f, -1.0f), 
            glm::vec3 p_up = glm::vec3(0.0f, 1.0f, 0.0f), 
            GLfloat p_yaw = -90.0f, GLfloat p_pitch = 0.0f, 
            float p_speed = 10.0f, float p_sensitivity = 0.1f,
            float p_origin_mouse_x = 0.0f, float p_origin_mouse_y = 0.0f,
            float p_max_mouse_x = 1280.0f, float p_max_mouse_y = 720.0f);

        void set_mouse(float p_mouse_x, float p_mouse_y);
        void set_position(glm::vec3 p_position);

        void forward(float p_time_delta, GLboolean p_lock_y = true);
        void backward(float p_time_delta, GLboolean p_lock_y = true);
        void right(float p_time_delta, GLboolean p_lock_y = true);
        void left(float p_time_delta, GLboolean p_lock_y = true);

        glm::vec3 get_position() 
        {
            return m_position;
        }

        glm::mat4 get_view();

        void set_origin(float p_origin_mouse_x, float p_origin_mouse_y)
        {
            m_origin_mouse_x = p_origin_mouse_x;
            m_origin_mouse_y = p_origin_mouse_y;
        }

    private:

        void update();

        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_global_up;

        GLfloat m_yaw;
        GLfloat m_pitch;

        float m_speed;
        float m_sensitivity;
        float m_zoom;

        float m_origin_mouse_x;
        float m_origin_mouse_y;

        float m_max_mouse_x;
        float m_max_mouse_y;
    };
};

#endif