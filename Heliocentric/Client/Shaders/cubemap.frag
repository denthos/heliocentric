#version 430 core

in vec3 TexCoord;

layout (location = 0) out vec4 color;
layout (location = 1) out vec4 buffer1_color;

uniform samplerCube cubemap;

void main(){
	color = texture(cubemap, TexCoord);
	buffer1_color = vec4(vec3(0.0), 1.0);
}
