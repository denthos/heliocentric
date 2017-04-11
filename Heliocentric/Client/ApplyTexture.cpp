#include "ApplyTexture.h"

#include "ShaderUtils.h"

bool ApplyTexture::init = false;
GLuint ApplyTexture::textureShader;

ApplyTexture::ApplyTexture(GLuint texture) : texture(texture) {
	if (!init) {
		compileShaders("Shaders/simpleTexture.vert", "Shaders/simpleTexture.frag", &textureShader);
	}
}

void ApplyTexture::draw(GLuint shader, glm::mat4 C) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	Group::draw(textureShader, C);
	glBindTexture(GL_TEXTURE_2D, 0);
}