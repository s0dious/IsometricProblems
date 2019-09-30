#ifndef _ISO_MODEL_HPP
#define _ISO_MODEL_HPP

#include "glad/glad.h"
#include "glm/vec3.hpp"

namespace iso
{
    class Material
    {
    public:
        Material(glm::vec3 p_ambient, glm::vec3 p_diffuse, glm::vec3 p_specular, GLfloat p_shininess);

        void set_ambient(glm::vec3 p_ambient);
        void set_diffuse(glm::vec3 p_diffuse);
        void set_specular(glm::vec3 p_specular);
        void set_shininess(GLfloat p_shininess);
        
        glm::vec3 get_ambient();
        glm::vec3 get_diffuse();
        glm::vec3 get_specular();
        GLfloat get_shininess();

    private:
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
        GLfloat m_shininess;
    };

    class Light
    {
    public:
        Light(glm::vec3 p_position, glm::vec3 p_ambient, glm::vec3 p_diffuse, glm::vec3 p_specular);

        void set_position(glm::vec3 p_position);
        void set_ambient(glm::vec3 p_ambient);
        void set_diffuse(glm::vec3 p_diffuse);
        void set_specular(glm::vec3 p_specular);

        glm::vec3 get_position();
        glm::vec3 get_ambient();
        glm::vec3 get_diffuse();
        glm::vec3 get_specular();

    private:
        glm::vec3 m_position;
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
    };
}

#endif