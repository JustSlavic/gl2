#version 330 core

layout (location = 0) in vec4 position;
out vec2 v_texture_coordinates;

void main()
{
    v_texture_coordinates = vec2(position.x, position.y);
    gl_Position = position;
}
