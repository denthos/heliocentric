#pragma once
#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "shader.h"
#include "texture.h"
#include "camera.h"

class PlayerIcon {
public:
	PlayerIcon( Shader* shader); //init fields
	~PlayerIcon();
	void update(const glm::vec3 &position);
	void draw(const Camera &camera);
	glm::mat4 world_mat;
	void setColor(glm::vec4 color);
	void setSize(float size);
	static bool drawIcons;

private:
	Shader * shader;
	float size;
	GLuint VBO, VAO, icon_info_buffer;
	glm::vec4 color;
	
};