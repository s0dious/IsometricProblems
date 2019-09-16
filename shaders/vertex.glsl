#version 330 core

// Input
layout (location = 0) in vec3 aPos;

// Output
out vec4 VertexColor;

void main() 
{
    gl_Position = vec4(aPos, 1.0f);
    VertexColor = vec4(aPos, 1.0f);
}