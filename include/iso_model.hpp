#ifndef _ISO_MODEL_HPP
#define _ISO_MODEL_HPP

#include "iso_shader.hpp"

namespace iso
{
    class Model
    {

    };

    class Material
    {
    public:
        Material(glm::vec3 p_ambient, glm::vec3 p_diffuse, glm::vec3 p_specular, GLfloat p_shininess)
        {
            m_ambient = p_ambient;
            m_diffuse = p_diffuse;
            m_specular = p_specular;
            m_shininess = p_shininess;
        }

        void set_ambient(glm::vec3 p_ambient)
        {
            m_ambient = p_ambient;
        }

        void set_diffuse(glm::vec3 p_diffuse)
        {
            m_diffuse = p_diffuse;
        }

        void set_sepcular(glm::vec3 p_specular)
        {
            m_specular = p_specular;
        }

        void set_shininess(GLfloat p_shininess)
        {
            m_shininess = p_shininess;
        }

        void apply(ShaderProgram p_shader_program)
        {
            p_shader_program.set_uniform("material.ambient", m_ambient);
            p_shader_program.set_uniform("material.diffuse", m_diffuse);
            p_shader_program.set_uniform("material.specular", m_specular);
            p_shader_program.set_uniform("material.shininess", m_shininess);
        }
    private:
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
        GLfloat m_shininess;
    };

    class Light
    {
    public:
        Light(glm::vec3 p_position, glm::vec3 p_ambient, glm::vec3 p_diffuse, glm::vec3 p_specular)
        {
            m_position = p_position;
            m_ambient = p_ambient;
            m_diffuse = p_diffuse;
            m_specular = p_specular;
        }

        void set_position(glm::vec3 p_position)
        {
            m_position = p_position;
        }

        void set_ambient(glm::vec3 p_ambient)
        {
            m_ambient = p_ambient;
        }

        void set_diffuse(glm::vec3 p_diffuse)
        {
            m_diffuse = p_diffuse;
        }

        void set_sepcular(glm::vec3 p_specular)
        {
            m_specular = p_specular;
        }

        void apply(ShaderProgram p_shader_program)
        {
            p_shader_program.set_uniform("light.position", m_position);
            p_shader_program.set_uniform("light.ambient", m_ambient);
            p_shader_program.set_uniform("light.diffuse", m_diffuse);
            p_shader_program.set_uniform("light.specular", m_specular);
        }

    private:
        glm::vec3 m_position;
        glm::vec3 m_ambient;
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
    };
}

#endif