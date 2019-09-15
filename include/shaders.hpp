#ifndef _SHADERS_HPP
#define _SHADERS_HPP

#include <string>
#include <glad/glad.h> 

namespace iso
{
    class Shader
    {
    public:
        Shader(std::string p_file, GLenum p_shader_type);
        GLuint compile();

    private:
        std::string m_file;
        GLenum m_shader_type;
        GLuint m_shader_id;
    };
}

#endif