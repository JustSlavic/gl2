#version 330 core

layout (location = 0) in vec3 position;

out vec2 v_texture_coordinates;

void main()
{
    v_texture_coordinates = position.xy;
    gl_Position = vec4(position, 1.0);
}
