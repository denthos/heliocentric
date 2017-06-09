#version 430 core

layout (location = 0) in vec2 position;
layout (location = 2) in vec4 color;

uniform vec3 icon_offset;
uniform vec4 player_color;

out VS_OUT {
    vec4 color;
} vs_out;

void main()
{
    gl_Position =  vec4(icon_offset.xy, 0.0f, 1.0f); 
    vs_out.color = player_color * color;
}


