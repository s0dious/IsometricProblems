
// C/C++ Libraries
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

// SFML Libraries
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

// OpenGL Libraries
#include <glad/glad.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project files
#include "iso_camera.hpp"
#include "iso_database.hpp"
#include "iso_input.hpp"
#include "iso_map.hpp"
#include "iso_physics.hpp"
#include "iso_shader.hpp"

#include "octree/octree.h"

int main()
{
    // Set some OpenGL context settings
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2;

    // Target OpenGL version 3.3 Core
    settings.majorVersion = 3;
    settings.minorVersion = 3;
    settings.attributeFlags = sf::ContextSettings::Core;

    // Create OpenGL context with SFML
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "OpenGL", sf::Style::None, settings);
    // window.setFramerateLimit(60);

    // GLAD will find the proper opengl functions at runtime for cross platform compatability
    gladLoadGL();

    // Load shaders
    iso::VertexShader vertex_shader("shaders/voxel_vertex.glsl");
    vertex_shader.get_error();

    iso::FragmentShader fragment_shader("shaders/voxel_fragment.glsl");
    fragment_shader.get_error();

    iso::ShaderProgram voxel_shader;
    voxel_shader.attach_shader(vertex_shader);
    voxel_shader.attach_shader(fragment_shader);
    voxel_shader.link();

    // Define vertex and index data
    iso::VoxelMap game_map(128);

    iso::MaterialModel material(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(1.0f, 0.2f, 0.3f), glm::vec3(0.5f, 0.5f, 0.2f), 1.0f);
    uint material_id = game_map.add_material(material);

    for(GLint i = 0; i < 128; i++)
    {
        for(GLint j = 0; j < 128; j++)
        {
            // std::cout << "Adding " << i << " " << j << std::endl;
            game_map(i, 0, j) = material_id;
        }
    }

    // Intialize controllers
    iso::InputController input_controller(window);
    iso::CharacterController character_controller;
    iso::PhysicsController physics_controller;
    iso::CameraController camera_controller;

    // iso::shader_id_t voxel_shader_id = camera_controller.set_shader(voxel_shader);
    camera_controller.add_shader(voxel_shader);

    std::vector<iso::Character> game_characters;

    iso::CharacterModel character_model(iso::PhysicsModel(10.0f, 0.8f, 0.5f, 1.0f, 0.3f, 0.7f, 1.0f));
    iso::Character character(character_model,
                            glm::vec3(64.0f, 2.0f, 64.0f),
                            iso::InputType::Keyboard,
                            iso::CameraType::ThirdPerson);

    game_characters.push_back(character);

    // Add the character to the camera
    iso::Drawable game_character_drawable = game_characters[0].get_drawable();
    iso::drawable_id_t game_character_drawable_id = camera_controller.add_drawable(game_character_drawable);
    // camera_controller.add_drawable(game_character_drawable);

    // // Initialize window state
    std::vector<iso::Drawable> game_map_drawable = game_map.get_drawable();
    std::cout << game_map_drawable.size() << std::endl;
    camera_controller.add_drawable(game_map_drawable);

    std::cout << "here1" << std::endl;

    glEnable(GL_DEPTH_TEST);

    // Initialize game state
    sf::Clock game_clock;

    float current_time = 0;
    float previous_time = 0;

    glm::vec3 light_position(64.0, 25.0f, 64.0f);

    // Event loop where all the magic happens
    while(window.isOpen())
    {
        current_time = game_clock.getElapsedTime().asSeconds();
        float time_delta = current_time - previous_time;
        previous_time = current_time;

        // Display game state
        float fps = 1.0f / time_delta;

        if(true)
        {
            std::cout << "FPS: " << fps << std::endl;
        }

        input_controller.update(game_characters);
        character_controller.update_input(game_characters, time_delta);
        physics_controller.update(game_characters, time_delta);

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update drawables
        game_character_drawable = game_characters[0].get_drawable();
        camera_controller.update_drawable(game_character_drawable, game_character_drawable_id);

        // Draw drawables
        glm::vec3 current_light_position(light_position.x + 5*sin(current_time) , light_position.y, light_position.z + 5*cos(current_time));
        iso::LightModel light(current_light_position, glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        camera_controller.draw(game_characters[0].get_camera(), light);

        window.display();
    }

    return 0;
}