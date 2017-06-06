#pragma once
#include "glm\glm.hpp"
#include "shader.h"
#include "texture.h"
#include "camera.h"

struct Icon {
	glm::vec3 pos;
	glm::vec4 color;
	float size;
};

class PlayerIcon {
public:
	PlayerIcon( Shader* shader); //init fields
	~PlayerIcon();
	
	void draw(const Camera &camera, const glm::mat4 &toWorld);
	glm::mat4 world_mat;

private:
	Shader * shader;
	GLuint VBO, VAO, icon_info_buffer;

	Icon icon;



};