
// C/C++ Libraries
#include <iostream>
#include <string>
#include <fstream>

// SFML Libraries
#include <SFML/Window.hpp>

// OpenGL Libraries
#include <glad/glad.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project files
#include "shaders.hpp"

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
    sf::Window window(sf::VideoMode(800, 800), "OpenGL", sf::Style::Default, settings);

    // GLAD will find the proper opengl functions at runtime for cross platform compatability
    gladLoadGL();

    // Load shaders
    std::string vertex_shader_string = load_file(std::string("shaders/vertex.glsl"));
    std::string fragment_shader_string = load_file(std::string("shaders/fragment.glsl"));

    const GLchar* vertex_shader_source = vertex_shader_string.c_str();
    const GLchar* fragment_shader_source = fragment_shader_string.c_str();

    GLint success;
    GLchar info_log[512];

    // Load vertex shader and make sure it compiles
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);

    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // Load fragment shader and make sure it compiles
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
    }

    // Make a shader program, link shaders, and check for linking errors
    GLuint shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, info_log);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
    }

    // Define vertex and index data
    GLfloat vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Create vertex array and buffer objects
    GLuint vertex_array_object, vertex_buffer_object, element_buffer_object;

    glGenVertexArrays(1, &vertex_array_object);
    glGenBuffers(1, &vertex_buffer_object);
    glGenBuffers(1, &element_buffer_object);

    // Bind vertex array, buffers, and set attributes
    glBindVertexArray(vertex_array_object);

    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer_object);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer_object);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);


    // Initialize game state
    sf::Clock game_clock;
    sf::Clock game_state_printout_timer;
    glm::vec3 game_position(0.0f, 0.0f, 0.0f);

    // Event loop where all the magic happens
    while(window.isOpen()) 
    {
        // Update game state
        sf::Event event;
        glm::vec3 position_delta(0.0f, 0.0f, 0.0f);
        sf::Time time_elapsed = game_clock.getElapsedTime();

        sf::Time time_delta = game_state_printout_timer.getElapsedTime();

        // std::cout << time_delta.asSeconds() << std::endl;

        // Display game state
        if(time_delta.asSeconds() > 1.0f)
        {
            std::cout << time_elapsed.asSeconds() << std::endl;
            std::cout << "<" << game_position.x << ", " << game_position.y << ", " << game_position.z << ">" << std::endl;
            
            std::cout << std::endl;
            game_state_printout_timer.restart();
        }

        while(window.pollEvent(event)) 
        {

            // Process events
            if(event.type == sf::Event::Closed) {
                window.close();
            }
            else if(event.type == sf::Event::KeyPressed) {
                std::cout << "Key pressed" << std::endl;

                switch(event.key.code)
                {
                    case sf::Keyboard::Escape :
                        window.close();
                        break;
                    case sf::Keyboard::Up :
                        position_delta += glm::vec3(0.0f, 0.1f, 0.0f);
                        break;
                    case sf::Keyboard::Right :
                        position_delta += glm::vec3(0.1f, 0.0f, 0.0f);
                        break;
                    case sf::Keyboard::Down :
                        position_delta += glm::vec3(0.0f, -0.1f, 0.0f);
                        break;
                    case sf::Keyboard::Left :
                        position_delta += glm::vec3(-0.1f, 0.0f, 0.0f);
                        break;
                    default:
                        break;
                }

                // std::cout << "<" << position_delta.x << ", " << position_delta.y << ", " << position_delta.z << ">" << std::endl;
            }

            game_position += position_delta;
        }
        

        // Clear screen
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Create transformations
        glm::mat4 transform = glm::mat4(1.0f);

        // transform = glm::translate(transform, game_position);
        transform = glm::translate(transform, game_position);
        // transform = glm::rotate(transform, (float)time_elapsed.asSeconds(), glm::vec3(0.0f, 0.0f, 1.0f));

        // Draw objects
        glUseProgram(shader_program);

        unsigned int transformLoc = glGetUniformLocation(shader_program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        for(uint8_t i = 0; i < 4; i++)
        {
            for(uint8_t j = 0; j < 4; j++)
            {
                std::cout << transform[i][j] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;

        glBindVertexArray(vertex_array_object);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Push buffer to screen
        window.display();
    }

    return 0;
}