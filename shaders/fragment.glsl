#version 330 core

// Input
in vec2 VertexColor;

// Output
out vec4 FragColor;

void main() {
    // FragColor = vec4(0.0f, 0.0f, 0.0f, 5.0f);
    if(VertexColor.x < 0.05f || VertexColor.x > 0.95f || VertexColor.y < 0.05f || VertexColor.y > 0.95f)
    {
        FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        FragColor = vec4(0.55f, 0.33f, 0.07f, 1.0f);
    }
}