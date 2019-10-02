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
}