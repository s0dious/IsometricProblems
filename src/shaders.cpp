#include "shaders.hpp"

iso::Shader::Shader(std::string p_file, GLenum p_shader_type)
{
    m_file = p_file;
    m_shader_type = p_shader_type;
    m_shader_id = glCreateShader(p_shader_type);
}

GLuint iso::Shader::compile()
{
    return 1;
}