#pragma once
#include "glm\glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
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
	void update(const glm::vec3 &position);
	void draw(const Camera &camera);
	glm::mat4 world_mat;

private:
	Shader * shader;
	GLuint VBO, VAO, icon_info_buffer;

	Icon icon;



};