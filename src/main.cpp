
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

    iso::MaterialModel material(glm::vec3(1.0f, 0.0f, 0.2f), glm::vec3(1.0f, 0.2f, 0.3f), glm::vec3(0.5f, 0.5f, 0.2f), 32.0f);
    uint material_id = game_map.add_material(material);

    for(GLint i = 0; i < 128; i++)
    {
        for(GLint j = 0; j < 128; j++)
        {
            std::cout << "Adding " << i << " " << j << std::endl;
            game_map(i, j, 0) = material_id;
        }
    }

    // Intialize controllers
    iso::InputController input_controller(window);
    iso::CharacterController character_controller;
    iso::PhysicsController physics_controller;
    std::cout << "here1" << std::endl;
    iso::CameraController camera_controller(voxel_shader);
    std::cout << "here2" << std::endl;

    std::vector<iso::Character> game_characters;

    iso::CharacterModel character_model(iso::PhysicsModel(10.0f, 0.8f, 0.5f, 1.0f, 0.3f, 0.7f, 1.0f));
    iso::Character character(character_model,
                            glm::vec3(64.0f, 2.0f, 64.0f),
                            iso::InputType::Keyboard,
                            iso::CameraType::ThirdPerson);

    game_characters.push_back(character);



    // // Initialize window state
    std::vector<iso::Drawable> game_map_drawable = game_map.get_drawable();
    camera_controller.set(game_map_drawable);

    glEnable(GL_DEPTH_TEST);

    // Initialize game state
    sf::Clock game_clock;

    float current_time = 0;
    float previous_time = 0;

    glm::vec3 light_position(64.0, 5.0f, 64.0f);

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

        camera_controller.add(character.get_drawable());
        glm::vec3 current_light_position(light_position.x + 5*sin(current_time) , light_position.y, light_position.z + 5*cos(current_time));
        iso::LightModel light(current_light_position, glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
        camera_controller.draw(character.get_camera(), light);

        // // Activate our shader program
        // voxel_shader.use();
        // voxel_shader.set_uniform("viewPos", camera_controller.get_position(game_characters[0]));

        // voxel_shader.apply(material);
        // voxel_shader.apply(light);
        
        // // Perspective math
        // glm::mat4 view = glm::mat4(1.0f);
        // glm::mat4 projection = glm::mat4(1.0f);
        // projection = glm::perspective(glm::radians(45.0f), screen_width/screen_height, 0.1f, 100.0f);
        // view = camera_controller.get_view(game_characters[0]);

        // glm::mat4 model = glm::mat4(1.0f);

        // voxel_shader.set_uniform("projection", projection);
        // voxel_shader.set_uniform("view", view);
        // voxel_shader.set_uniform("model", model);

        // game_map.draw(camera_controller.get_position(game_characters[0]));

        window.display();
    }

    return 0;
}