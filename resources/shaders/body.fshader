#version 330 core

in vec2 v_texture_coordinates;

uniform float u_radius;

out vec4 out_color;

void main()
{
    vec2 uv = v_texture_coordinates;

    vec2 zo = vec2(0.f, 1.f);
    float r = length(uv);
    out_color = vec4(zo.xxx, smoothstep(.5f, .5f - .01f, r));
} 
