#ifndef _ISO_SHADER_HPP
#define _ISO_SHADER_HPP

#include <string>
#include <fstream>
#include <iostream>
#include <vector>

#include <glad/glad.h> 
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace iso
{
    class Shader
    {
    public:
        Shader(std::string p_file, GLenum p_shader_type);
        void get_error(std::ostream& err = std::cerr);
        GLuint get_shader_id();
        
    private:
        std::string m_file;
        std::string m_shader_source;
        GLenum m_shader_type;
        GLuint m_shader_id;
    };

    class VertexShader : public Shader
    {
    public:
        VertexShader(std::string p_file) : Shader(p_file, GL_VERTEX_SHADER) {}
    };

    class FragmentShader : public Shader
    {
    public:
        FragmentShader(std::string p_file) : Shader(p_file, GL_FRAGMENT_SHADER) {}
    };

    class ShaderProgram
    {
    public:
        ShaderProgram();
        void attach_shader(Shader& p_shader);
        void link();
        void get_error(std::ostream& err = std::cerr);
        void use();

        void set_uniform(std::string p_uniform, glm::mat4 mat_four);
        void set_uniform(std::string p_uniform, glm::vec3 vec_three);
        void set_uniform(std::string p_uniform, float f);

    private:
        GLuint m_shader_program_id;

        std::vector<Shader> m_shaders;
    };
}

#endif