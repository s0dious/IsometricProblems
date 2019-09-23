#include "iso_shader.hpp"

namespace iso 
{
    /**
     * SHADER IMPLEMENTATION
     * 
     */

    Shader::Shader(std::string p_file, GLenum p_shader_type)
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


    void Shader::get_error(std::ostream& err)
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


    GLuint Shader::get_shader_id()
    {
        return m_shader_id;
    }

    /**
     * 
     * SHADER PROGRAM IMPLEMENTATION
     * 
     * ShaderProgram    - Constructor
     * attach_shader    - Attach a Shader object to the shader program
     * get_error        - Print any errors encountered to the stream provided (cerr by default)
     * use              - Activate the shader program for rendering
     * set_uniform      - Overloaded, set the uniform to the provided value
     * 
     * apply_material   - Activate the provided material for rendering
     * apply_light      - Activate the provided light for rendering
     * 
     */

    // Core functionality

    ShaderProgram::ShaderProgram()
    {
        m_shader_program_id = glCreateProgram();
    }


    void ShaderProgram::attach_shader(Shader& p_shader)
    {
        m_shaders.push_back(p_shader);
        glAttachShader(m_shader_program_id, p_shader.get_shader_id());
    }


    void ShaderProgram::link()
    {
        glLinkProgram(m_shader_program_id);
    }


    void ShaderProgram::get_error(std::ostream& err)
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


    void ShaderProgram::use()
    {
        glUseProgram(m_shader_program_id);
    }


    // Render option methods

    /**
     * Test comment
     */
    void ShaderProgram::apply_material(iso::Material material)
    {
        material.app
    }


    void ShaderProgram::apply_light(iso::Light light)
    {
        set_uniform("light.position", light.get_position());
        set_uniform("light.ambient", light.get_ambient());
        set_uniform("light.diffuse", light.get_diffuse());
        set_uniform("light.specular", light.get_specular());
    }

    // Set uniform methods

    void ShaderProgram::set_uniform(std::string p_uniform, glm::mat4 mat_four)
    {
        GLuint uniform_id = glGetUniformLocation(m_shader_program_id, p_uniform.c_str());
        glUniformMatrix4fv(uniform_id, 1, GL_FALSE, glm::value_ptr(mat_four));
    }


    void ShaderProgram::set_uniform(std::string p_uniform, glm::vec3 vec_three)
    {
        GLuint uniform_id = glGetUniformLocation(m_shader_program_id, p_uniform.c_str());
        glUniform3fv(uniform_id, 1, &vec_three[0]);
    }


    void ShaderProgram::set_uniform(std::string p_uniform, float f)
    {
        GLuint uniform_id = glGetUniformLocation(m_shader_program_id, p_uniform.c_str());
        glUniform1f(uniform_id, f);
    }
}
