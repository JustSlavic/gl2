#version 330 core

#define PI 3.14159265359

in vec2 v_texture_coordinates;

uniform float u_scale_factor;
// uniform float u_width;
// uniform float u_length;

out vec4 out_color;


mat2 rotate (float alpha) {
    float c = cos(alpha);
    float s = sin(alpha);

    return mat2(c, -s, s, c);
}


float to_radians (float alpha) {
    return alpha * PI / 180.f;
}


float arrow_tip (vec2 uv, float alpha, float beta, float len) {
    float brightness;
    float back;
    vec2 n;
    
    n = vec2(cos(alpha), sin(alpha));
    brightness = smoothstep(0.0f, 0.01f, dot(uv - vec2(len, 0.f), -n.yx));
    
    n = vec2(cos(alpha), -sin(alpha));
    brightness *= smoothstep(0.0f, 0.01f, dot(uv - vec2(len, 0.f),  n.yx));
    
    n = vec2(cos(beta), sin(beta));
    back = smoothstep(0.0f, 0.01f, dot(uv, n.yx));
    
    n = vec2(cos(beta), -sin(beta));
    back += smoothstep(0.0f, 0.01f, dot(uv, -n.yx));
    
    return brightness * clamp(back, 0.f, 1.f);
}


float arrow_tail (vec2 uv, float len, float width) {
    float hw = width * .5f; // half width
    
    float brightness = 1.f;
    
    brightness *= smoothstep(0.f, 0.01f, uv.x + len); // vertical left line
    brightness *= smoothstep(0.f, 0.01f, -uv.x + 0.01); // vertical right line
    brightness *= smoothstep(0.f, 0.01f, uv.y + hw); // horizontal bottom line
    brightness *= smoothstep(0.f, 0.01f, -uv.y + hw); // horizontal upper line
    
    return brightness;
}


void main () {
    vec2 uv = v_texture_coordinates;
    float tip_length = 0.5f;
    float tail_length = 1.f - tip_length;
    float tail_width = 0.1f;

    float tip = arrow_tip(uv, to_radians(15.f), to_radians(60.f), tip_length);
    float tail = arrow_tail(uv, tail_length, tail_width);
    float b = clamp(tip + tail, 0.f, 1.f);

    vec3 color = vec3(1.f, 1.f, 0.f); // yellow

    out_color = vec4(color * b, b);
} 
