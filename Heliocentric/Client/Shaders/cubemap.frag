#version 430 core

in vec3 TexCoord;
out vec4 color;

uniform samplerCube cubemap;

void main(){
	color = texture(cubemap, TexCoord);
}
