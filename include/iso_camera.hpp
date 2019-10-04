#ifndef _ISO_CAMERA_HPP
#define _ISO_CAMERA_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "iso_database.hpp"
#include "iso_shader.hpp"

namespace iso
{
    class Drawable
    {
    public:
        Drawable(iso::MaterialModel p_material = iso::MaterialModel(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 0.0f)):
            m_data(0),
            m_indices(0),
            m_material(p_material)
        {

        }

        std::vector<GLfloat> get_data() const
        {
            return m_data;
        }

        std::vector<GLint> get_indices() const
        {
            return m_indices;
        }

        MaterialModel get_material() const
        {
            return m_material;
        }

    protected:
        std::vector<GLfloat> m_data;
        std::vector<GLint> m_indices;
        MaterialModel m_material;
    };


    class Camera
    {
    public:
        virtual glm::mat4 get_view() = 0;
        virtual glm::vec3 get_position() = 0; 
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

            std::cout << "a" << std::endl;
            std::vector<GLfloat> data = p_drawable.get_data();
            std::vector<GLint> indices = p_drawable.get_indices();
            std::cout << "b" << std::endl;

            if(data.size() > 0 && indices.size() > 0)
            {
                glGenVertexArrays(1, &vertex_array_object);
                glGenBuffers(1, &vertex_buffer_object);
                glGenBuffers(1, &element_buffer_object);

                glBindVertexArray(vertex_array_object);

                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
                glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), (void*)data.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), (void*)indices.data(), GL_STATIC_DRAW);

                // Location data uniform
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
                glEnableVertexAttribArray(0);

                // Normal data uniform
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), ((void*)(3*sizeof(float))));
                glEnableVertexAttribArray(1);

                m_set_objects.push_back(p_drawable);
                m_set_vertex_arrays.push_back(vertex_array_object);

                glBindVertexArray(0);
            }
        }

        void add(const Drawable& p_drawable)
        {
            m_add_objects.push_back(p_drawable);
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

        void draw(Camera& p_camera)
        {
            m_shader.use();

            // Set light
            LightModel light = m_lights[0];

            m_shader.set_uniform("light.position", light.position);
            m_shader.set_uniform("light.ambient", light.ambient);
            m_shader.set_uniform("light.diffuse", light.diffuse);
            m_shader.set_uniform("light.specular", light.specular);

            // Set the camera

            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
            glm::mat4 view = glm::mat4(1.0f);

            projection = glm::perspective(glm::radians(45.0f), 1920.0f/1280.0f, 0.1f, 100.0f);
            view = p_camera.get_view();

            m_shader.set_uniform("viewPos", p_camera.get_position());

            m_shader.set_uniform("projection", projection);
            m_shader.set_uniform("view", view);
            m_shader.set_uniform("model", model);

            // Draw set objects
            for(std::vector<Drawable>::size_type i = 0; i < m_set_objects.size(); i++)
            {
                Drawable& current_object = m_set_objects[i];
                GLuint current_vertex_array = m_set_vertex_arrays[i];
                MaterialModel current_material = current_object.get_material();

                glBindVertexArray(current_vertex_array);

                m_shader.set_uniform("material.ambient", current_material.ambient);
                m_shader.set_uniform("material.diffuse", current_material.diffuse);
                m_shader.set_uniform("material.specular", current_material.specular);
                m_shader.set_uniform("material.shininess", current_material.shininess);

                glDrawElements(GL_TRIANGLES, current_object.get_indices().size(), GL_UNSIGNED_INT, (void*)0);
            }


            // Draw added objects
            for(std::vector<Drawable>::size_type i = 0; i < m_add_objects.size(); i++)
            {
                Drawable& current_object = m_add_objects[i];
                MaterialModel current_material = current_object.get_material();

                std::vector<GLfloat> current_data = current_object.get_data();
                std::vector<GLint> current_indices = current_object.get_indices();

                GLuint vertex_array_object, vertex_buffer_object, element_buffer_object;

                glGenVertexArrays(1, &vertex_array_object);
                glGenBuffers(1, &vertex_buffer_object);
                glGenBuffers(1, &element_buffer_object);

                glBindVertexArray(vertex_array_object);

                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
                glBufferData(GL_ARRAY_BUFFER, current_data.size() * sizeof(float), (void*)current_data.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, current_indices.size() * sizeof(GLuint), (void*)current_indices.data(), GL_STATIC_DRAW);

                // Location data uniform
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
                glEnableVertexAttribArray(0);

                // Normal data uniform
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), ((void*)(3*sizeof(float))));
                glEnableVertexAttribArray(1);

                m_shader.set_uniform("material.ambient", current_material.ambient);
                m_shader.set_uniform("material.diffuse", current_material.diffuse);
                m_shader.set_uniform("material.specular", current_material.specular);
                m_shader.set_uniform("material.shininess", current_material.shininess);

                glDrawElements(GL_TRIANGLES, current_indices.size(), GL_UNSIGNED_INT, (void*)0);

                glBindVertexArray(0);
                glDeleteBuffers(1, &vertex_buffer_object);
                glDeleteBuffers(1, &element_buffer_object);
                glDeleteVertexArrays(1, &vertex_array_object);
            }
            m_add_objects.clear();
        }

    private:
        std::vector<LightModel> m_lights;

        std::vector<Drawable> m_set_objects;
        std::vector<Drawable> m_add_objects;

        std::vector<GLuint> m_set_vertex_arrays;

        std::vector<std::vector<GLfloat> > m_data;
        std::vector<std::vector<GLint> > m_indices;

        iso::ShaderProgram m_shader;
    };
};

#endif