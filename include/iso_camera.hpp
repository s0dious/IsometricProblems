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

        Drawable():
            data(0),
            indices(0),
            material()
        {

        }

        Drawable(std::vector<GLfloat> p_data, std::vector<GLint> p_indices, iso::MaterialModel p_material):
            data(p_data),
            indices(p_indices),
            material(p_material)
        {
            
        }
    };


    struct Camera
    {
        glm::mat4 view;
        glm::vec3 position;

        Camera(glm::mat4 p_view, glm::vec3 p_position):
            view(p_view),
            position(p_position)
        {

        }
    };


    class CameraController
    {
    public:
        CameraController(iso::ShaderProgram p_shader):
            m_shader(p_shader)
        {

        }

        void set(const Drawable& p_drawable)
        {
            GLuint vertex_array_object, vertex_buffer_object, element_buffer_object;

            if(p_drawable.data.size() > 0 && p_drawable.indices.size() > 0)
            {
                glGenVertexArrays(1, &vertex_array_object);
                glGenBuffers(1, &vertex_buffer_object);
                glGenBuffers(1, &element_buffer_object);

                glBindVertexArray(vertex_array_object);

                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
                glBufferData(GL_ARRAY_BUFFER, p_drawable.data.size() * sizeof(float), (void*)p_drawable.data.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_drawable.indices.size() * sizeof(GLuint), (void*)p_drawable.indices.data(), GL_STATIC_DRAW);

                // Location data uniform
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
                glEnableVertexAttribArray(0);

                // Normal data uniform
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), ((void*)(3*sizeof(float))));
                glEnableVertexAttribArray(1);

                m_set_objects.push_back(p_drawable);
                m_set_vertex_arrays.push_back(vertex_array_object);

                std::cout << "drawable set" << std::endl;

                glBindVertexArray(0);
            }
        }

        void add(const Drawable& p_drawable)
        {
            if(p_drawable.data.size() > 0 && p_drawable.indices.size() > 0)
            {
                m_add_objects.push_back(p_drawable);
                std::cout << "drawable added" << std::endl;
            }
        }

        void add(const std::vector<Drawable>& p_drawables)
        {
            for(std::vector<Drawable>::size_type i = 0; i < p_drawables.size(); i++)
            {
                add(p_drawables[i]);
            }
        }

        void set(const std::vector<Drawable>& p_drawables)
        {
            for(std::vector<Drawable>::size_type i = 0; i < p_drawables.size(); i++)
            {
                std::cout << i << std::endl;
                set(p_drawables[i]);
            }
        }

        void draw(Camera p_camera, iso::LightModel p_light)
        {
            m_shader.use();

            // Set light
            m_shader.set_uniform("light.position", p_light.position);
            m_shader.set_uniform("light.ambient", p_light.ambient);
            m_shader.set_uniform("light.diffuse", p_light.diffuse);
            m_shader.set_uniform("light.specular", p_light.specular);

            std::cout << "Light set" << std::endl;

            // Set the camera

            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);

            projection = glm::perspective(glm::radians(45.0f), 1920.0f/1280.0f, 0.1f, 100.0f);
            m_shader.set_uniform("viewPos", p_camera.position);

            m_shader.set_uniform("projection", projection);
            m_shader.set_uniform("view", p_camera.view);
            m_shader.set_uniform("model", model);

            std::cout << "Camera set" << std::endl;

            // Draw set objects
            for(std::vector<Drawable>::size_type i = 0; i < m_set_objects.size(); i++)
            {
                iso::Drawable& current_object = m_set_objects[i];
                GLuint current_vertex_array = m_set_vertex_arrays[i];

                glBindVertexArray(current_vertex_array);

                m_shader.set_uniform("material.ambient", current_object.material.ambient);
                m_shader.set_uniform("material.diffuse", current_object.material.diffuse);
                m_shader.set_uniform("material.specular", current_object.material.specular);
                m_shader.set_uniform("material.shininess", current_object.material.shininess);

                glDrawElements(GL_TRIANGLES, current_object.indices.size(), GL_UNSIGNED_INT, (void*)0);

                std::cout << "Drawing " << current_object.indices.size() / 3 << " triangles" << std::endl;
            }
            std::cout << m_set_objects.size() << " set drawables drawn" << std::endl;


            // Draw added objects
            for(std::vector<Drawable>::size_type i = 0; i < m_add_objects.size(); i++)
            {
                Drawable& current_object = m_add_objects[i];

                GLuint vertex_array_object, vertex_buffer_object, element_buffer_object;

                glGenVertexArrays(1, &vertex_array_object);
                glGenBuffers(1, &vertex_buffer_object);
                glGenBuffers(1, &element_buffer_object);

                glBindVertexArray(vertex_array_object);

                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
                glBufferData(GL_ARRAY_BUFFER, current_object.data.size() * sizeof(float), (void*)current_object.data.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, current_object.indices.size() * sizeof(GLuint), (void*)current_object.indices.data(), GL_STATIC_DRAW);

                // Location data uniform
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
                glEnableVertexAttribArray(0);

                // Normal data uniform
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), ((void*)(3*sizeof(float))));
                glEnableVertexAttribArray(1);

                m_shader.set_uniform("material.ambient", current_object.material.ambient);
                m_shader.set_uniform("material.diffuse", current_object.material.diffuse);
                m_shader.set_uniform("material.specular", current_object.material.specular);
                m_shader.set_uniform("material.shininess", current_object.material.shininess);

                glDrawElements(GL_TRIANGLES, current_object.indices.size(), GL_UNSIGNED_INT, (void*)0);

                glBindVertexArray(0);
                glDeleteBuffers(1, &vertex_buffer_object);
                glDeleteBuffers(1, &element_buffer_object);
                glDeleteVertexArrays(1, &vertex_array_object);
            }
            m_add_objects.clear();

            std::cout << m_add_objects.size() << " added drawables drawn" << std::endl;
        }

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