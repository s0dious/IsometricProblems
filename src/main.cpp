
// C/C++ Libraries
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

// SFML Libraries
#include <SFML/Window.hpp>

// OpenGL Libraries
#include <glad/glad.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project files
#include "iso_camera.hpp"
#include "iso_shader.hpp"
#include "iso_map.hpp"

#include "octree/octree.h"

std::string load_file(std::string filename)
{
    std::string file_string;

    std::ifstream file(filename);
    if(file.is_open())
    {
        std::string temp;
        while(std::getline(file, temp))
        {
            file_string += temp + "\n";
        }

        // file_string += "\0";
    }

    return file_string;
}


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
    sf::Window window(sf::VideoMode(1280, 720), "OpenGL", sf::Style::Default, settings);

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

    // A cube
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    // Octree<glm::vec3> game_map(128);
    iso::Map<glm::vec3> game_map(128);

    for(GLint i = 0; i < 128; i++)
    {
        for(GLint j = 0; j < 128; j++)
        {
            game_map.data(i, j, 0) = glm::vec3((float)i, -2.0f, (float)j);
        }
    }

    // Create vertex array and buffer objects
    GLuint vertex_array_object, vertex_buffer_object; //, element_buffer_object;

    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &vertex_buffer_object);
    // glGenBuffers(1, &element_buffer_object);

    // Bind vertex array, buffers, and set attributes
    glBindVertexArray(vertex_array_object);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), ((void*)(3*sizeof(float))));
    glEnableVertexAttribArray(1);

    // Initialize window state
    float screen_width = 1280.0f;
    float screen_height = 720.0f;

    glEnable(GL_DEPTH_TEST);

    // Initialize game state
    sf::Clock game_clock;

    float current_time = 0;
    float previous_time = 0;

    glm::vec3 light_position(0.0, 5.0f, 0.0f);

    GLuint frames_per_print = 200;
    GLuint frames_print_count = frames_per_print;

    iso::Camera camera(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    camera.set_origin(screen_width/2, screen_height/2);

    camera.set_mouse((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y);

    // Event loop where all the magic happens
    while(window.isOpen()) 
    {
        // std::cout << frames_print_count << std::endl;
        // Update game state
        sf::Event event;
        glm::vec3 camera_position_delta(0.0f, 0.0f, 0.0f);
        camera.set_mouse((float)sf::Mouse::getPosition(window).x, (float)sf::Mouse::getPosition(window).y);

        // std::cout << (float)sf::Mouse::getPosition(window).x << " " << (float)sf::Mouse::getPosition(window).y << std::endl;

        current_time = game_clock.getElapsedTime().asSeconds();
        float time_delta = current_time - previous_time;
        previous_time = current_time;

        // std::cout << time_delta.asSeconds() << std::endl;


        // Display game state
        float fps = 1.0f / time_delta;

        if(--frames_print_count == 0)
        {
            std::cout << "FPS: " << fps << std::endl;
            std::cout << "(" << camera.get_position().x << ", " << camera.get_position().y << ", " << camera.get_position().z << ")" << std::endl;
            
            frames_print_count = frames_per_print;
        }
        

        while(window.pollEvent(event)) 
        {

            // Process events
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            else if(event.type == sf::Event::KeyPressed) {
                switch(event.key.code)
                {
                    case sf::Keyboard::Escape :
                        window.close();
                        break;
                }
            }

            
        }

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            camera.forward(time_delta);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            camera.right(time_delta);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            camera.left(time_delta);
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            camera.backward(time_delta);
        }


        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate our shader program
        voxel_shader.use();
        voxel_shader.set_uniform("light.position", glm::vec3(light_position.x + 5*sin(current_time) , light_position.y, light_position.z + 5*cos(current_time)));
        voxel_shader.set_uniform("viewPos", camera.get_position());

        voxel_shader.set_uniform("light.ambient", glm::vec3(1.0f, 1.0f, 1.0f));
        voxel_shader.set_uniform("light.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
        voxel_shader.set_uniform("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

        voxel_shader.set_uniform("material.ambient", glm::vec3(1.0f, 0.0f, 0.3f));
        voxel_shader.set_uniform("material.diffuse", glm::vec3(1.0f, 0.2f, 0.3f));
        voxel_shader.set_uniform("material.specular", glm::vec3(0.5f, 0.5f, 0.2f));
        voxel_shader.set_uniform("material.shininess", 32.0f);
        
        // Perspective math
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), screen_width/screen_height, 0.1f, 100.0f);
        view = camera.get_view();

        voxel_shader.set_uniform("projection", projection);
        voxel_shader.set_uniform("view", view);

        glBindVertexArray(vertex_array_object);

        game_map.for_each(
            [&voxel_shader] (glm::vec3 position, glm::vec3 voxel_color) {

                // std::cout << "Running lambda at " << position.x << " " << position.z << std::endl;

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3((float)position.x, 0.0f, (float)position.y));
                voxel_shader.set_uniform("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }   
        );
        
        // Array2D<glm::vec3> map_slice;
        // for(size_t z = 0; z < 128; z++) {
        //     map_slice = game_map.zSlice(z);

        //     for(size_t y = 0; y < 128; y++) {

        //         for(size_t x = 0; x < 128; x++) {

        //             if(map_slice(x, y) != game_map.emptyValue())
        //             {
        //                 std::cout << "Voxel at " << x << " " << y << " " << z << std::endl;

        //                 glm::vec3 voxel_color = map_slice(x, y);

        //                 glm::mat4 model = glm::mat4(1.0f);
        //                 model = glm::translate(model, glm::vec3((float)x, 0.0f, (float)y));
        //                 voxel_shader.set_uniform("model", model);
        //                 glDrawArrays(GL_TRIANGLES, 0, 36);
        //             }
        //         }
        //     }
        // }

        // std::cout << "x y position:" << (float)sf::Mouse::getPosition(window).x << " " << (float)sf::Mouse::getPosition(window).y << std::endl;

        window.display();
        // std::cout << std::endl;
    }

    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);

    return 0;
}