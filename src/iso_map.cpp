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

    std::vector<VoxelSet> VoxelMap::get_drawable()
    {
        size_t material_count = m_materials.size();
        std::cout << "material count: " << material_count << std::endl;

        std::vector<std::vector<glm::vec3>> map_voxels(material_count);

        for(size_t i = 0; i < m_size; i++)
        {
            for(size_t j = 0; j < m_size; j++)
            {
                for(size_t k = 0; k < m_size; k++)
                {
                    uint current_material = m_data(i, j, k);

                    if(current_material <= material_count)
                    {
                        // std::cout << current_material << std::endl;
                        map_voxels[current_material].push_back(glm::vec3(i, k, j));
                    }
                } 
            }
        }

        std::cout << "map voxels gathered" << std::endl;

        std::vector<VoxelSet> drawable_map;

        for(size_t i = 0; i < material_count; i++)
        {
            std::cout << map_voxels[i].size() << " voxels of material " << i << std::endl;
            drawable_map.push_back(iso::VoxelSet(map_voxels[i], m_materials[i]));
        }

        std::cout << "Voxel sets created" << std::endl;

        return drawable_map;
    }
}