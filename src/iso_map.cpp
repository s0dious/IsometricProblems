#include "iso_map.hpp"

namespace iso
{
    VoxelMap::VoxelMap(size_t p_size):
        m_size(p_size),
        m_data(Octree<glm::vec4>(p_size))
    {
        glGenVertexArrays(1, &m_vertex_array_object);
        glGenBuffers(1, &m_vertex_buffer_object);
        glGenBuffers(1, &m_element_buffer_object);
    }

    void VoxelMap::draw(glm::vec3 p_camera)
    {
        if(!m_bound)
        {
            bind();
        }

        std::cout << "Drawing " << m_flat_data.size() << " voxels" << std::endl;
        std::cout << " - " << m_prepared_data.size() << " floats" << std::endl;
        std::cout << " - " << m_prepared_indices.size() << " indices" << std::endl;

        glDrawElements(GL_TRIANGLES, m_prepared_indices.size(), GL_UNSIGNED_INT, (void*)0);
    }

    glm::vec4& VoxelMap::operator()(size_t x, size_t y, size_t z)
    {
        return m_data(x, y, z);
    }

    void VoxelMap::buffer()
    {
        if(!m_prepared)
        {
            prepare();
        }
        
        // Bind vertex array, buffers, and set attributes
        glBindVertexArray(m_vertex_array_object);

        glBindBuffer(GL_ARRAY_BUFFER, m_vertex_buffer_object);
        glBufferData(GL_ARRAY_BUFFER, m_prepared_data.size() * sizeof(float), (void*)m_prepared_data.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_element_buffer_object);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_prepared_indices.size() * sizeof(GLuint), (void*)m_prepared_indices.data(), GL_STATIC_DRAW);

        // Location data uniform
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal data uniform
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), ((void*)(3*sizeof(float))));
        glEnableVertexAttribArray(1);

        m_buffered = true;
    }


    void VoxelMap::bind()
    {
        if(!m_buffered)
        {
            buffer();
        }

        glBindVertexArray(m_vertex_array_object);

        m_bound = true;
    }



    void VoxelMap::flatten()
    {
        for(size_t i = 0; i < m_size; i++)
        {
            for(size_t j = 0; j < m_size; j++)
            {
                for(size_t k = 0; k < m_size; k++)
                {
                    if(m_data(i, j, k).w != 0.0f)
                    {
                        m_flat_data.push_back(std::make_pair(glm::vec3((float)i, (float)j, (float)k), m_data(i, j, k)));
                    }
                }
            }
        }

        m_flat = true;
    }


    void VoxelMap::prepare()
    {
        if(!m_flat)
        {
            flatten();
        }
    
        float vertices[] = 
        {
            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,

            -0.5f, -0.5f,  0.5f,
            0.5f,  0.5f,  0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,

            -0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,

            0.5f,  0.5f,  0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,

            -0.5f, -0.5f, -0.5f,
            0.5f, -0.5f, -0.5f,
            0.5f, -0.5f,  0.5f,
            -0.5f, -0.5f,  0.5f,

            -0.5f,  0.5f, -0.5f,
            0.5f,  0.5f, -0.5f,
            0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f,
        };

        float normals[] = 
        {
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f,  1.0f,
            -1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f,  1.0f,  0.0f
        };

        int indices[] =
        {
            0, 1, 2,
            2, 3, 1,

            4, 5, 6,
            6, 7, 5,

            8, 9, 10,
            10, 11, 8,

            12, 13, 14,
            14, 15, 12,

            16, 17, 18,
            18, 19, 16,

            20, 21, 22,
            22, 23, 20
        };


        for(std::vector<std::pair<glm::vec3, glm::vec4> >::size_type i = 0; i < m_flat_data.size(); i++)
        {
            std::pair<glm::vec3, glm::vec4>& current_pair = m_flat_data[i];
            glm::vec3& current_position = current_pair.first;
            // glm::vec4& current_color = current_pair.second;

            for(size_t j = 0; j < 6; j++)
            {
                for(size_t k = 0; k < 4; k++)
                {
                    // Vertex
                    m_prepared_data.push_back(current_position.x + vertices[12*j + 3*k]);
                    m_prepared_data.push_back(current_position.z + vertices[12*j + 3*k + 1]);
                    m_prepared_data.push_back(current_position.y + vertices[12*j + 3*k + 2]);

                    // Normal
                    m_prepared_data.push_back(normals[j]);
                    m_prepared_data.push_back(normals[j]);
                    m_prepared_data.push_back(normals[j]);
                }
            }
            
            for(size_t j = 0; j < 36; j++)
            {
                m_prepared_indices.push_back(i*24 + indices[j]);
            }
        }

        m_prepared = true;
    }
}