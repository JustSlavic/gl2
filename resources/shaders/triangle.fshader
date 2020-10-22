#version 330 core

in vec2 v_texture_coordinates;

out vec4 out_color;

void main()
{
    vec2 uv = v_texture_coordinates;
    out_color = vec4(uv.xyx, 1.0f);
} 
