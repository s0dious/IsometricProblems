#include "iso_shader.hpp"

// Shader

iso::Shader::Shader(std::string p_file, GLenum p_shader_type)
{
    m_file = p_file;
    m_shader_type = p_shader_type;

    std::ifstream data_file(p_file);
    if(data_file)
    {
        data_file.seekg(std::ios::beg, std::ios::end);
        size_t len = data_file.tellg();
        data_file.seekg(std::ios::beg);

        m_shader_source = std::string(len + 1, '\0');
        data_file.read(&m_shader_source[0], len);
    
        data_file.close();

        const char* m_shader_source_str = m_shader_source.c_str();

        m_shader_id = glCreateShader(p_shader_type);
        glShaderSource(m_shader_id, 1, &m_shader_source_str, NULL);
        glCompileShader(m_shader_id);

        get_error(std::cout);
    }    
}

void iso::Shader::get_error(std::ostream& err)
{
    GLint success;
    GLchar info_log[512];

    // Load vertex shader and make sure it compiles
    glGetShaderiv(m_shader_id, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(m_shader_id, 512, NULL, info_log);
        err << "ERROR::ISO::SHADER::COMPILATION_FAILED\n" << info_log << std::endl;
    }
}

GLuint iso::Shader::get_shader_id()
{
    return m_shader_id;
}

// Shader Program

iso::ShaderProgram::ShaderProgram()
{
    m_shader_program_id = glCreateProgram();
}

void iso::ShaderProgram::attach_shader(Shader& p_shader)
{
    m_shaders.push_back(p_shader);
    glAttachShader(m_shader_program_id, p_shader.get_shader_id());
}

void iso::ShaderProgram::link()
{
    glLinkProgram(m_shader_program_id);
}

void iso::ShaderProgram::get_error(std::ostream& err)
{
    GLint success;
    GLchar info_log[512];

    // Load vertex shader and make sure it compiles
    glGetProgramiv(m_shader_program_id, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(m_shader_program_id, 512, NULL, info_log);
        err << "ERROR::ISO::SHADERPROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }
}

void iso::ShaderProgram::use()
{
    glUseProgram(m_shader_program_id);
}

void iso::ShaderProgram::set_uniform(std::string p_uniform, glm::mat4 mat_four)
{
    GLuint uniform_id = glGetUniformLocation(m_shader_program_id, p_uniform.c_str());
    glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(mat_four));
}

void iso::ShaderProgram::set_uniform(std::string p_uniform, glm::vec3 vec_three)
{
    GLuint uniform_id = glGetUniformLocation(m_shader_program_id, p_uniform.c_str());
    glUniform3fv(uniform_id, 1, &vec_three[0]);
}

void iso::ShaderProgram::set_uniform(std::string p_uniform, float f)
{
    GLuint uniform_id = glGetUniformLocation(m_shader_program_id, p_uniform.c_str());
    glUniform1f(uniform_id, f);
}