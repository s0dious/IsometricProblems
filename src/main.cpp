
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

    // A cube
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    std::vector<glm::vec3> game_map; 

    for(GLint i = -50; i <= 50; i++)
    {
        for(GLint j = -50; j <= 50; j++)
        {
            game_map.push_back(glm::vec3((float)i, 0.0f, (float)j));
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), ((void*)(3*sizeof(float))));
    glEnableVertexAttribArray(1);

    // Initialize window state
    float screen_width = 1280.0f;
    float screen_height = 720.0f;

    glEnable(GL_DEPTH_TEST);

    // Initialize game state
    sf::Clock game_clock;
    float previous_time = 0;

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

        std::cout << (float)sf::Mouse::getPosition(window).x << " " << (float)sf::Mouse::getPosition(window).y << std::endl;

        sf::Time current_time = game_clock.getElapsedTime();
        float time_delta = current_time.asSeconds() - previous_time;
        previous_time = current_time.asSeconds();

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
        glUseProgram(shader_program);
        
        // Perspective math
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), screen_width/screen_height, 0.1f, 100.0f);
        view = camera.get_view();

        GLuint projection_uniform = glGetUniformLocation(shader_program, "projection");
        glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection));
        
        GLuint view_uniform = glGetUniformLocation(shader_program, "view");
        glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(vertex_array_object);
        for(std::vector<glm::vec3>::size_type i = 0; i < game_map.size(); i++)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, game_map[i]);

            GLuint model_uniform = glGetUniformLocation(shader_program, "model");
            glUniformMatrix4fv(model_uniform, 1, GL_FALSE, glm::value_ptr(model));

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        // sf::Mouse::setPosition(sf::Vector2i(floorf(screen_width/2.0f), floorf(screen_height/2.0f)), window);
        std::cout << "x y position:" << (float)sf::Mouse::getPosition(window).x << " " << (float)sf::Mouse::getPosition(window).y << std::endl;

        window.display();
        std::cout << std::endl;
    }

    glDeleteVertexArrays(1, &vertex_array_object);
    glDeleteBuffers(1, &vertex_buffer_object);

    return 0;
}