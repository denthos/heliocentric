#version 430 core

layout (location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 TexCoord;

void main()
{

    mat4 view_no_translate = view;
    view_no_translate[3][0] = 0.0; 
    view_no_translate[3][1] = 0.0; 
    view_no_translate[3][2] = 0.0; 
    gl_Position = projection * view_no_translate * model * vec4(position, 1.0f);
    TexCoord = position;
}
