#version 430 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VERT_OUT {
	vec3 Normal;
	vec3 FragPos;
} vert_out;


void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
	vert_out.FragPos = vec3(model * vec4(position, 1.0f));
	vert_out.Normal = mat3(transpose(inverse(model))) * normal;
}
