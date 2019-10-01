#include <iso_camera.hpp>
#include <iostream>

#include "iso_camera.hpp"

namespace iso
{
    void CameraController::update(std::vector<iso::Character>& p_character_list)
    {
        // update the 
        // for(std::vector<iso::Character>::size_type i = 0; i < p_character_list.size(); i++)
        // {
        //     iso::Character& current_character = p_character_list[i];
        // }
    }


    glm::mat4 CameraController::get_view(const iso::Character& p_character)
    {
        glm::mat4 view_matrix;

        if(p_character.m_camera_type == iso::CameraType::ThirdPerson)
        {
            view_matrix = glm::lookAt(p_character.m_position, p_character.m_position + p_character.m_front, p_character.m_up);
        }
        
        return view_matrix;
    }

    glm::vec3 CameraController::get_position(const iso::Character& p_character)
    {
        glm::vec3 position_vector;

        if(p_character.m_camera_type == iso::CameraType::ThirdPerson)
        {
            std::cout << "3rd" << std::endl;
            position_vector = p_character.m_position;
        }
        
        return position_vector;
    }
}