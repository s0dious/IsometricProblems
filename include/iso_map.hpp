#ifndef _ISO_MAP_HPP
#define _ISO_MAP_HPP

#include <vector>
#include <functional>
#include <iostream>

#include "octree/octree.h"
#include "glad/glad.h"
#include "glm/vec3.hpp"

namespace iso
{
    template< typename T >
    class Map
    {
    public:
        Octree<T> data = Octree<T>(0);

        Map(GLuint p_size)
        {
            m_size = p_size;
            data = Octree<T>(p_size);
        }

        // void for_each(void (*p_func)(T p_data))
        void for_each(std::function<void(glm::vec3, T)> p_func)
        {
            if(!m_flattened)
            {
                flatten();
            }

            for(size_t i = 0; i < m_flat_data.size(); i++)
            {
                // std::cout << i << std::endl;
                p_func(m_flat_data[i].first, m_flat_data[i].second);
            }
        }

        void flatten()
        {
            for(GLuint i = 0; i < m_size; i++)
            {
                for(GLuint j = 0; j < m_size; j++)
                {
                    for(GLuint k = 0; k < m_size; k++)
                    {
                        if(data(i, j, k) != data.emptyValue())
                        {
                            m_flat_data.push_back(std::make_pair(glm::vec3((float)i, (float)j, (float)k), data(i, j, k)));
                        }
                    }
                }
            }

            std::cout << "Voxels found: " << m_flat_data.size() << std::endl;

            m_flattened = GL_TRUE;
        }

    private:
        GLuint m_size;
        std::vector<std::pair<glm::vec3, T> > m_flat_data;

        GLboolean m_flattened = GL_FALSE;
    };
}

#endif