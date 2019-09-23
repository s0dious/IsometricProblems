#include "iso_model.hpp"

namespace iso
{
    /*
     *  LIGHT IMPLEMENTATION
     * 
     */

    Light::Light(glm::vec3 p_position, glm::vec3 p_ambient, glm::vec3 p_diffuse, glm::vec3 p_specular)
    {
        m_position = p_position;
        m_ambient = p_ambient;
        m_diffuse = p_diffuse;
        m_specular = p_specular;
    }

    void Light::set_position(glm::vec3 p_position)
    {
        m_position = p_position;
    }

    glm::vec3 Light::get_position()
    {
        return m_position;
    }


    void Light::set_ambient(glm::vec3 p_ambient)
    {
        m_ambient = p_ambient;
    }

    glm::vec3 Light::get_ambient()
    {
        return m_ambient;
    }

    void Light::set_diffuse(glm::vec3 p_diffuse)
    {
        m_diffuse = p_diffuse;
    }

    glm::vec3 Light::get_diffuse()
    {
        return m_diffuse;
    }


    void Light::set_specular(glm::vec3 p_specular)
    {
        m_specular = p_specular;
    }

    glm::vec3 Light::get_specular()
    {
        return m_specular;
    }


    /*
     *  MATERIAL IMPLEMENTATION
     * 
     */

    Material::Material(glm::vec3 p_ambient, glm::vec3 p_diffuse, glm::vec3 p_specular, GLfloat p_shininess)
    {
        m_ambient = p_ambient;
        m_diffuse = p_diffuse;
        m_specular = p_specular;
        m_shininess = p_shininess;
    }

    void Material::set_ambient(glm::vec3 p_ambient)
    {
        m_ambient = p_ambient;
    }

    glm::vec3 Material::get_ambient()
    {
        return m_ambient;
    }


    void Material::set_diffuse(glm::vec3 p_diffuse)
    {
        m_diffuse = p_diffuse;
    }

    glm::vec3 Material::get_diffuse()
    {
        return m_diffuse;
    }


    void Material::set_specular(glm::vec3 p_specular)
    {
        m_specular = p_specular;
    }

    glm::vec3 Material::get_specular()
    {
        return m_specular;
    }


    void Material::set_shininess(GLfloat p_shininess)
    {
        m_shininess = p_shininess;
    }

    GLfloat Material::get_shininess()
    {
        return m_shininess;
    }
}