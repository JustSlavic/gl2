#version 330 core

layout (location = 0) in vec4 position;

out vec2 v_texture_coordinates;

uniform mat4 u_view;
uniform mat4 u_projection;

void main()
{
    vec4 p = u_projection * u_view * position;
    v_texture_coordinates = vec2(position.x, position.y);
    gl_Position = p;
}
