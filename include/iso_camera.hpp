#ifndef _ISO_CAMERA_HPP
#define _ISO_CAMERA_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "iso_database.hpp"
#include "iso_shader.hpp"

namespace iso
{
    typedef size_t shader_id_t;
    typedef std::pair<shader_id_t, size_t> drawable_id_t;

    struct Transform
    {
        glm::vec3 position;
        
        GLfloat pitch;
        GLfloat yaw;
        GLfloat roll;
        
        glm::vec3 scale;

        Transform(glm::vec3 p_position = glm::vec3(0.0f, 0.0f, 0.0f), 
                    GLfloat p_pitch = 0.0f, GLfloat p_yaw = 0.0f, GLfloat p_roll = 0.0f, 
                    glm::vec3 p_scale = glm::vec3(1.0f, 1.0f, 1.0f));
    };

    struct Drawable
    {
        iso::MaterialModel material;

        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
        std::vector<GLfloat> data;
        std::vector<GLint> indices;

        size_t frame = 0;
        std::vector<Transform> frames;

        Drawable();
        Drawable(const iso::MaterialModel& p_material, 
                    const std::vector<GLfloat>& p_data, 
                    const std::vector<GLint>& p_indices);
        Drawable(const iso::MaterialModel& p_material, 
                    const glm::vec3& p_position,
                    const std::vector<GLfloat>& p_data, 
                    const std::vector<GLint>& p_indices);
        Drawable(const iso::MaterialModel& p_material, 
                    const std::vector<GLfloat>& p_data, 
                    const std::vector<GLint>& p_indices,
                    const Transform& p_frame);
        Drawable(const iso::MaterialModel& p_material, 
                    const std::vector<GLfloat>& p_data, 
                    const std::vector<GLint>& p_indices,
                    const std::vector<Transform>& p_frames);
        Drawable(const iso::MaterialModel& p_material,
                    const GLfloat p_width,
                    const GLfloat p_height,
                    const GLfloat p_depth);
    };

    struct Camera
    {
        glm::mat4 view;
        glm::vec3 position;

        Camera(glm::mat4 p_view, glm::vec3 p_position);
    };


    class Cursor
    {
    public:
    private:

    };


    class CameraController
    {
    public:        
        drawable_id_t add_drawable(const iso::Drawable& p_drawable, shader_id_t p_shader_id = 0);
        drawable_id_t update_drawable(const iso::Drawable& p_drawable, drawable_id_t p_drawable_id);

        std::vector<drawable_id_t> add_drawable(const std::vector<Drawable>& p_drawables, shader_id_t p_shader_id = 0);

        shader_id_t add_shader(const iso::ShaderProgram p_shader);
        shader_id_t update_shader(const iso::ShaderProgram p_shader, shader_id_t p_shader_id);

        void clear(glm::vec3 p_clear_color);
        void draw(iso::Camera p_camera, iso::LightModel p_light);

    private:
        // Shader state
        std::vector<iso::ShaderProgram> m_shaders;

        /*
         *  Each of the following states are vectors of vectors
         * 
         *  The shader_id associated with the drawable is used to index into the vector to access the vector
         *      of drawables associated with the shader and its openGL handlers.
         */

        // Drawables state
        std::vector< std::vector<Drawable> > m_drawables = {};
        std::vector< std::vector<GLboolean> > m_drawable_updated = {};

        // OpenGL state
        std::vector< std::vector<GLuint> > m_vertex_arrays = {};
        std::vector< std::vector<GLuint> > m_vertex_buffers = {};
        std::vector< std::vector<GLuint> > m_element_buffers = {};
    };
};

#endif