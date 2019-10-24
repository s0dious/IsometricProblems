#include "iso_camera.hpp"
#include <iostream>

namespace iso
{
    Transform::Transform(glm::vec3 p_position, 
                    GLfloat p_pitch, GLfloat p_yaw, GLfloat p_roll, 
                    glm::vec3 p_scale):
        position(p_position),
        pitch(p_pitch),
        yaw(p_yaw),
        roll(p_roll),
        scale(p_scale)
    {

    }

    Drawable::Drawable():
        frames(1)
    { 

    }

    Drawable::Drawable(const iso::MaterialModel& p_material, 
                const std::vector<GLfloat>& p_data, 
                const std::vector<GLint>& p_indices):
        material(p_material),
        data(p_data),
        indices(p_indices),
        frames(1)
    { 
    }

    Drawable::Drawable(const iso::MaterialModel& p_material,
                const glm::vec3& p_position,
                const std::vector<GLfloat>& p_data, 
                const std::vector<GLint>& p_indices):
        material(p_material),
        position(p_position),
        data(p_data),
        indices(p_indices),
        frames(1)
    { 
        frames[0].position = p_position;
    }

    Drawable::Drawable(const iso::MaterialModel& p_material, 
                    const std::vector<GLfloat>& p_data, 
                    const std::vector<GLint>& p_indices,
                    const Transform& p_frame):
        material(p_material),
        data(p_data),
        indices(p_indices),
        frames({p_frame})
    { }

    Drawable::Drawable(const iso::MaterialModel& p_material, 
                const std::vector<GLfloat>& p_data, 
                const std::vector<GLint>& p_indices,
                const std::vector<Transform>& p_frames):
        material(p_material),
        data(p_data),
        indices(p_indices),
        frames(p_frames)
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

                return std::make_pair(p_shader_id, m_drawables[p_shader_id].size()-1);
            }
        }

        return std::make_pair(p_shader_id, -1);
    }


    std::vector<drawable_id_t> CameraController::add_drawable(const std::vector<Drawable>& p_drawables, shader_id_t p_shader_id)
    {   
        std::vector<drawable_id_t> drawable_ids;

        std::cout << "Adding " << p_drawables.size() << " drawables" << std::endl;

        for(std::vector<Drawable>::size_type i = 0; i < p_drawables.size(); i++)
        {
            drawable_id_t index = add_drawable(p_drawables[i], p_shader_id);
            drawable_ids.push_back(index);
            std::cout << i << std::endl;
        }

        return drawable_ids;
    }


    drawable_id_t CameraController::update_drawable(const iso::Drawable& p_drawable, drawable_id_t p_drawable_id)
    {
        shader_id_t shader_id = p_drawable_id.first;
        size_t drawable_index = p_drawable_id.second;

        // If it's a valid id then update the drawable and the opengl buffer
        if(shader_id < m_drawables.size() && drawable_index < m_drawables[shader_id].size())
        {
            GLuint vertex_array_object, vertex_buffer_object, element_buffer_object;

            vertex_array_object = m_vertex_arrays[shader_id][drawable_index];
            vertex_buffer_object = m_vertex_buffers[shader_id][drawable_index];
            element_buffer_object = m_element_buffers[shader_id][drawable_index];

            if(p_drawable.data.size() > 0 && p_drawable.indices.size() > 0)
            {
                glBindVertexArray(vertex_array_object);

                glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
                glBufferData(GL_ARRAY_BUFFER, p_drawable.data.size() * sizeof(GLfloat), (void*)p_drawable.data.data(), GL_STATIC_DRAW);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, p_drawable.indices.size() * sizeof(GLuint), (void*)p_drawable.indices.data(), GL_STATIC_DRAW);

                glBindVertexArray(0);

                // Update stored drawables
                m_drawables[shader_id][drawable_index] = p_drawable;

                return p_drawable_id;
            }
        }

        return std::make_pair(-1, -1);
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

            glm::mat4 projection = glm::mat4(1.0f);
            glm::mat4 view = p_camera.view;

            projection = glm::perspective(glm::radians(45.0f), 1920.0f/1280.0f, 0.1f, 100.0f);
            m_shaders[i].set_uniform("viewPos", p_camera.position);

            m_shaders[i].set_uniform("projection", projection);
            m_shaders[i].set_uniform("view", view);
            // m_shader.set_uniform("model", model);

            std::cout << "Camera set" << std::endl;

            // Draw set objects
            for(std::vector<Drawable>::size_type j = 0; j < m_drawables[i].size(); j++)
            {
                iso::Drawable& current_drawable = m_drawables[i][j];
                GLuint current_vertex_array = m_vertex_arrays[i][j];

                glBindVertexArray(current_vertex_array);

                m_shaders[i].set_uniform("material.ambient", current_drawable.material.ambient);
                m_shaders[i].set_uniform("material.diffuse", current_drawable.material.diffuse);
                m_shaders[i].set_uniform("material.specular", current_drawable.material.specular);
                m_shaders[i].set_uniform("material.shininess", current_drawable.material.shininess);

                std::cout << "frame " << current_drawable.frame << " of " << current_drawable.frames.size() << std::endl;

                glm::mat4 model = glm::mat4(1.0f);
                iso::Transform& transform = current_drawable.frames[current_drawable.frame];
                current_drawable.frame = (current_drawable.frame + 1) % current_drawable.frames.size();
                std::cout << transform.pitch << " " << transform.yaw << " " << transform.roll << std::endl;
                std::cout << glm::radians(transform.pitch) << " " << glm::radians(transform.yaw) << " " << glm::radians(transform.roll) << std::endl;
                model = glm::translate(model, transform.position);
                model = glm::scale(model, transform.scale);
                model = glm::rotate(model, glm::radians(transform.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(transform.yaw), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(transform.roll), glm::vec3(0.0f, 0.0f, 1.0f));
                // if(current_drawable.position != glm::vec3(0.0f, 0.0f, 0.0f))
                // {
                //     model = glm::translate(model, current_drawable.position);
                // }
                // else
                // {
                //     model = glm::mat4(1.0f);
                // }
                m_shaders[i].set_uniform("model", model);

                glDrawElements(GL_TRIANGLES, current_drawable.indices.size(), GL_UNSIGNED_INT, (void*)0);

                glBindVertexArray(0);
            }
        }
    }
}