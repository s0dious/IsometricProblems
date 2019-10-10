#include "iso_camera.hpp"
#include <iostream>

namespace iso
{
    Drawable::Drawable()
    { }

    Drawable::Drawable(const iso::MaterialModel& p_material, 
                const std::vector<GLfloat>& p_data, 
                const std::vector<GLint>& p_indices):
        material(p_material),
        data(p_data),
        indices(p_indices)
    { 
        std::cout << "constructor" << std::endl;
    }

    Drawable::Drawable(const iso::MaterialModel& p_material,
                const glm::vec3& p_position,
                const std::vector<GLfloat>& p_data, 
                const std::vector<GLint>& p_indices):
        material(p_material),
        position(p_position),
        data(p_data),
        indices(p_indices)
    { }

    Drawable::Drawable(const iso::MaterialModel& p_material, 
                const std::vector<GLfloat>& p_data, 
                const std::vector<GLint>& p_indices,
                const std::vector<GLint>& p_angles,
                const glm::vec3& p_origin,
                const glm::vec3& p_axis):
        material(p_material),
        data(p_data),
        indices(p_indices),
        angles(p_angles),
        origin(p_origin),
        axis(p_axis)
    { }

    Camera::Camera(glm::mat4 p_view, glm::vec3 p_position):
        view(p_view),
        position(p_position)
    { }

    
    /**
     *
     *  CameraController Implementation
     * 
     */

    drawable_id_t CameraController::add_drawable(const iso::Drawable& p_drawable, shader_id_t p_shader_id)
    {
        if(p_shader_id < m_shaders.size())
        {
            GLuint vertex_array_object, vertex_buffer_object, element_buffer_object;

            if(p_drawable.data.size() > 0 && p_drawable.indices.size() > 0)
            {
                glGenVertexArrays(1, &vertex_array_object);
                glGenBuffers(1, &vertex_buffer_object);
                glGenBuffers(1, &element_buffer_object);

                glBindVertexArray(vertex_array_object);

                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
                glBufferData(GL_ARRAY_BUFFER, p_drawable.data.size() * sizeof(GLfloat), (void*)p_drawable.data.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_drawable.indices.size() * sizeof(GLuint), (void*)p_drawable.indices.data(), GL_STATIC_DRAW);

                // Location data uniform
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
                glEnableVertexAttribArray(0);

                // Normal data uniform
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), ((void*)(3*sizeof(GLfloat))));
                glEnableVertexAttribArray(1);

                m_drawables[p_shader_id].push_back(p_drawable);
                m_vertex_arrays[p_shader_id].push_back(vertex_array_object);
                m_vertex_buffers[p_shader_id].push_back(vertex_buffer_object);
                m_element_buffers[p_shader_id].push_back(element_buffer_object);

                glBindVertexArray(0);

                return std::make_pair(p_shader_id, m_drawables[p_shader_id].size()-1); // m_drawables.size() - 1;
            }
        }

        return std::make_pair(p_shader_id, -1);
    }


    std::vector<drawable_id_t> CameraController::add_drawable(const std::vector<Drawable>& p_drawables, shader_id_t p_shader_id)
    {   
        std::vector<drawable_id_t> drawable_ids;

        for(std::vector<Drawable>::size_type i = 0; i < p_drawables.size(); i++)
        {
            drawable_id_t index = add_drawable(p_drawables[i], p_shader_id);
            drawable_ids.push_back(index);
            std::cout << i << std::endl;
        }

        return drawable_ids;
    }


    shader_id_t CameraController::add_shader(const iso::ShaderProgram p_shader)
    {
        m_shaders.push_back(p_shader);
        
        std::vector<iso::Drawable> shader_drawables;
        std::vector<GLuint> shader_vertex_arrays;
        std::vector<GLuint> shader_vertex_buffers;
        std::vector<GLuint> shader_element_buffers;

        m_drawables.push_back(shader_drawables);
        m_vertex_arrays.push_back(shader_vertex_arrays);
        m_vertex_buffers.push_back(shader_vertex_buffers);
        m_element_buffers.push_back(shader_element_buffers);

        return m_shaders.size() - 1;
    }


    void CameraController::draw(iso::Camera p_camera, iso::LightModel p_light)
    {
        for(size_t i = 0; i < m_shaders.size(); i++)
        {
            m_shaders[i].use();

            // Set light
            m_shaders[i].set_uniform("light.position", p_light.position);
            m_shaders[i].set_uniform("light.ambient", p_light.ambient);
            m_shaders[i].set_uniform("light.diffuse", p_light.diffuse);
            m_shaders[i].set_uniform("light.specular", p_light.specular);

            // std::cout << "position" << p_light.position.x << " " << p_light.position.y << " " << p_light.position.z  << std::endl;

            std::cout << "Light set" << std::endl;

            // Set the camera

            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 projection = glm::mat4(1.0f);
            glm::mat4 view = p_camera.view;

            projection = glm::perspective(glm::radians(45.0f), 1920.0f/1280.0f, 0.1f, 100.0f);
            m_shaders[i].set_uniform("viewPos", p_camera.position);

            m_shaders[i].set_uniform("projection", projection);
            m_shaders[i].set_uniform("view", view);
            // m_shader.set_uniform("model", model);

            std::cout << "Camera set" << std::endl;

            // Draw set objects
            for(std::vector<Drawable>::size_type j = 0; j < m_drawables.size(); j++)
            {
                iso::Drawable& current_drawable = m_drawables[i][j];
                GLuint current_vertex_array = m_vertex_arrays[i][j];

                glBindVertexArray(current_vertex_array);

                m_shaders[i].set_uniform("material.ambient", current_drawable.material.ambient);
                m_shaders[i].set_uniform("material.diffuse", current_drawable.material.diffuse);
                m_shaders[i].set_uniform("material.specular", current_drawable.material.specular);
                m_shaders[i].set_uniform("material.shininess", current_drawable.material.shininess);

                if(current_drawable.position != glm::vec3(0.0f, 0.0f, 0.0f))
                {
                    model = glm::translate(model, current_drawable.position);
                }
                else
                {
                    model = glm::mat4(1.0f);
                }
                m_shaders[i].set_uniform("model", model);

                glDrawElements(GL_TRIANGLES, current_drawable.indices.size(), GL_UNSIGNED_INT, (void*)0);

                glBindVertexArray(0);
            }
        }
    }
}