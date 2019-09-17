#version 330 core

// Input
layout (location = 0) in vec3 aPos;

uniform mat4 transform;

// Output
out vec4 VertexColor;

void main() 
{
    gl_Position = transform * vec4(aPos, 1.0f);
    VertexColor = vec4(aPos, 1.0f);
}