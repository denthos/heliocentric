#version 430 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec3 offset;
layout (location = 2) in vec4 color;
layout (location = 3) in float scale;

uniform mat4 model;
uniform mat4 view;

out VS_OUT {
    vec4 color;
    float size;
} vs_out;

void main()
{
    gl_Position = view * model * vec4((position.xy + offset.xy), offset.z, 1.0f); 
    vs_out.color = color;
    vs_out.size = scale;
}

