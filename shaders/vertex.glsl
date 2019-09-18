#version 330 core

// Input
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Output
out vec2 VertexColor;

void main() 
{
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    VertexColor = aColor;
}