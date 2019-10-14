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

const float c_vertices[72] = 
{
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
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
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    10, 11, 8,

    12, 13, 14,
    14, 15, 12,

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20
};

namespace iso
{
    class VoxelMap
    {
    public:
        VoxelMap(size_t p_size);
        uint& operator()(size_t x, size_t y, size_t z); 
        iso::MaterialModel get_material(uint i);
        uint add_material(iso::MaterialModel p_material);
        std::vector<Drawable> get_drawable();

    private:
        size_t m_size;
        std::vector<iso::MaterialModel> m_materials;
        Octree<uint> m_data;
    };
}

#endif