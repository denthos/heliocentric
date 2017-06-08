#version 430 core

layout (location = 0) in vec2 position;
layout (location = 2) in vec4 color;
//layout (location = 3) in float scale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 offset;

out VS_OUT {
    vec4 color;
   // float size;
} vs_out;

void main()
{
    gl_Position =  vec4(offset.xy, 0.0f, 1.0f); 
    vs_out.color = color;
    //vs_out.size = scale;
}


