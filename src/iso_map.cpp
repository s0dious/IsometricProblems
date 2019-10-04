#include "iso_map.hpp"

namespace iso
{
    VoxelMap::VoxelMap(size_t p_size):
        m_size(p_size),
        m_data(Octree<uint>(p_size))
    {
        m_data.setEmptyValue(-1);
    }

    uint& VoxelMap::operator()(size_t x, size_t y, size_t z)
    {
        return m_data(x, y, z);
    }

    iso::MaterialModel VoxelMap::get_material(uint i)
    {
        return m_materials[i];
    }

    uint VoxelMap::add_material(iso::MaterialModel p_material)
    {
        m_materials.push_back(p_material);

        return m_materials.size() - 1;
    }

    std::vector<Drawable> VoxelMap::get_drawable()
    {
        size_t material_count = m_materials.size();
        std::cout << "material count: " << material_count << std::endl;

        std::vector<Drawable> map_drawables(material_count);

        for(size_t x = 0; x < m_size; x++)
        {
            for(size_t y = 0; y < m_size; y++)
            {
                for(size_t z = 0; z < m_size; z++)
                {
                    uint current_material = m_data(x, y, z);

                    if(current_material <= material_count)
                    {
                        iso::Drawable& current_drawable = map_drawables[current_material];

                        // set material
                        current_drawable.material = m_materials[current_material];

                        // set data
                        size_t current_end_index = current_drawable.data.size()/6;
                        for(size_t j = 0; j < 6; j++)
                        {
                            for(size_t k = 0; k < 4; k++)
                            {
                                // Vertex
                                current_drawable.data.push_back((float)x + c_vertices[12*j + 3*k]);
                                current_drawable.data.push_back((float)y + c_vertices[12*j + 3*k + 1]);
                                current_drawable.data.push_back((float)z + c_vertices[12*j + 3*k + 2]);

                                // std::cout << (float)x + c_vertices[12*j + 3*k] << " " << (float)y + c_vertices[12*j + 3*k + 1] << " " << (float)z + c_vertices[12*j + 3*k + 2] << std::endl;

                                // Normal
                                current_drawable.data.push_back(c_normals[3*j]);
                                current_drawable.data.push_back(c_normals[3*j + 1]);
                                current_drawable.data.push_back(c_normals[3*j + 2]);
                            }
                            // std::cout << std::endl;
                        }
                        
                        // set indices
                        for(size_t j = 0; j < 36; j++)
                        {
                            current_drawable.indices.push_back(current_end_index + c_indices[j]);
                            // std::cout << current_end_index + c_indices[j] << std::endl;
                        }
                    }
                } 
            }
        }

        std::cout << "map voxels gathered" << std::endl;

        return map_drawables;
    }
}