#ifndef _ISO_CAMERA_HPP
#define _ISO_CAMERA_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "iso_database.hpp"
#include "iso_shader.hpp"

namespace iso
{
    struct Drawable
    {
        std::vector<GLfloat> data;
        std::vector<GLint> indices;
        iso::MaterialModel material;

        Drawable();
        Drawable(std::vector<GLfloat> p_data, std::vector<GLint> p_indices, iso::MaterialModel p_material);
    };


    struct Camera
    {
        glm::mat4 view;
        glm::vec3 position;

        Camera(glm::mat4 p_view, glm::vec3 p_position);
    };


    class CameraController
    {
    public:
        CameraController(iso::ShaderProgram p_shader);

        void set(const Drawable& p_drawable);
        void set(const std::vector<Drawable>& p_drawables);

        void add(const Drawable& p_drawable);
        void add(const std::vector<Drawable>& p_drawables);

        void draw(iso::Camera p_camera, iso::LightModel p_light);

    private:
        std::vector<Drawable> m_set_objects;
        std::vector<Drawable> m_add_objects;

        std::vector<GLuint> m_set_vertex_arrays;

        std::vector<std::vector<GLfloat> > m_data;
        std::vector<std::vector<GLint> > m_indices;

        iso::ShaderProgram m_shader;
    };
};

#endif