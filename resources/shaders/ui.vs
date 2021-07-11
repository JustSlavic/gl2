#version 330 core

layout (location = 0) in vec2 position;
out vec2 v_texture_coordinates;

void main()
{
    v_texture_coordinates = position;
    gl_Position = vec4(position.x, position.y, 0, 1);
}
