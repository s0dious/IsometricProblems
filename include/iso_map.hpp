#ifndef _ISO_MAP_HPP
#define _ISO_MAP_HPP

#include <vector>
#include <functional>
#include <iostream>

#include "octree/octree.h"
#include "glad/glad.h"
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "iso_camera.hpp"
#include "iso_database.hpp"

namespace iso
{
    class Voxel : public iso::Drawable
    {
    public:
        Voxel(glm::vec3 p_pos)
        {
            for(size_t j = 0; j < 6; j++)
            {
                for(size_t k = 0; k < 4; k++)
                {
                    // Vertex
                    m_data.push_back(p_pos.x + c_vertices[12*j + 3*k]);
                    m_data.push_back(p_pos.z + c_vertices[12*j + 3*k + 1]);
                    m_data.push_back(p_pos.y + c_vertices[12*j + 3*k + 2]);

                    // Normal
                    m_data.push_back(c_normals[j]);
                    m_data.push_back(c_normals[j]);
                    m_data.push_back(c_normals[j]);
                }
            }
            
            for(size_t j = 0; j < 36; j++)
            {
                m_indices.push_back(c_indices[j]);
            }
        }

    private:
        const float c_vertices[72] = 
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

        const float c_normals[18] = 
        {
            0.0f,  0.0f, -1.0f,
            0.0f,  0.0f,  1.0f,
            -1.0f,  0.0f,  0.0f,
            1.0f,  0.0f,  0.0f,
            0.0f, -1.0f,  0.0f,
            0.0f,  1.0f,  0.0f
        };

        const uint c_indices[36] =
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
    };


    class VoxelSet : public iso::Drawable
    {
    public:
        VoxelSet(std::vector<glm::vec3> p_voxels, iso::MaterialModel p_material):
            Drawable(p_material)
        {
            m_material = p_material;
            for(std::vector<glm::vec3>::size_type i = 0; i < p_voxels.size(); i++)
            {
                Voxel current_voxel(p_voxels[i]);
                std::vector<GLfloat> current_data = current_voxel.get_data();
                std::vector<GLint> current_indices = current_voxel.get_indices();

                m_data.insert(m_data.end(), current_data.begin(), current_data.end());
                m_indices.insert(m_indices.end(), current_indices.begin(), current_indices.end());
            }
        }
    };

    class VoxelMap
    {
    public:
        VoxelMap(size_t p_size);
        uint& operator()(size_t x, size_t y, size_t z); 
        iso::MaterialModel get_material(uint i);
        uint add_material(iso::MaterialModel p_material);
        std::vector<VoxelSet> get_drawable();

    private:
        size_t m_size;
        std::vector<iso::MaterialModel> m_materials;
        Octree<uint> m_data;
        std::vector<VoxelSet> m_drawable_data;
    };
}

#endif