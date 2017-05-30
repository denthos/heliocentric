#include "drawable.h"

#ifdef _DEBUG
#include <glad\glad.h>
#define BB_SHADER_VERT "bounding_box.vert"
#define BB_SHADER_FRAG "bounding_box.frag"
#define DRAW_BOUNDING_BOXES true
GLuint bbVAO, bbVBO, bbEBO;
bool _init = false;
void init() {
	if (!_init) {
		glGenVertexArrays(1, &bbVAO);
		glGenBuffers(1, &bbVBO);
		glGenBuffers(1, &bbEBO);

		glBindVertexArray(bbVAO);

		// Vertices
		const GLfloat vertices[8][3] = {
			// bottom square
			{ 0.0f, 0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 1.0f, 0.0f, 0.0f },{ 1.0f, 0.0f, 1.0f },
			// top square
			{ 0.0f, 1.0f, 0.0f },{ 0.0f, 1.0f, 1.0f },{ 1.0f, 1.0f, 0.0f },{ 1.0f, 1.0f, 1.0f }
		};

		glBindBuffer(GL_ARRAY_BUFFER, bbVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		// Indices
		const GLuint indices[12][2] = {
			// draw bottom square
			{ 0, 1 },{ 1, 3 },{ 3, 2 },{ 2, 0 },
			// draw top square
			{ 4, 5 },{ 5, 7 },{ 7, 6 },{ 6, 4 },
			// connect them
			{ 0, 4 },{ 1, 5 },{ 2, 6 },{ 3, 7 }
		};

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bbEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		_init = true;
	}
}
#endif

Drawable::Drawable() : toWorld(glm::mat4(1.0f)) {

}

void Drawable::draw(const Camera & camera) const {
	model->draw(*shader, camera, toWorld);
#ifdef _DEBUG
	if (DRAW_BOUNDING_BOXES) {
		init();
		static Shader bbShader = Shader(BB_SHADER_VERT, BB_SHADER_FRAG);
		bbShader.bind();
		BoundingBox bb = model->getBoundingBox();
		glm::mat4 transform = toWorld * glm::translate(glm::mat4(1.0f), bb.min) * glm::scale(glm::mat4(1.0f), bb.max - bb.min);
		glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "view"), 1, GL_FALSE, &camera.view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "projection"), 1, GL_FALSE, &camera.perspective[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(shader->getPid(), "model"), 1, GL_FALSE, &transform[0][0]);
		glLineWidth(1.0f);
		glBindVertexArray(bbVAO);
		glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		bbShader.unbind();
	}
#endif
}

void Drawable::update() {
	model->update();
}

BoundingBox Drawable::getBoundingBox() const {
	BoundingBox boundingBox = model->getBoundingBox();
	glm::vec4 v = toWorld * glm::vec4(boundingBox.min, 1.0f);
	boundingBox.min = glm::vec3(v);
	v = toWorld * glm::vec4(boundingBox.max, 1.0f);
	boundingBox.max = glm::vec3(v);
	return boundingBox;
}

bool Drawable::intersect(const Ray & ray, Collision & collision) const {
	Ray newRay;
	Collision newCollision;
	glm::mat4 inverse = glm::inverse(toWorld);
	newRay.origin = glm::vec3(inverse * glm::vec4(ray.origin, 1.0f));
	newRay.direction = glm::vec3(inverse * glm::vec4(ray.direction, 0.0f));
	if (model->intersect(newRay, newCollision)) {
		glm::vec3 collision_position = glm::vec3(toWorld * glm::vec4(newCollision.position, 1.0f)); 
		float collision_distance = glm::distance(ray.origin, collision_position);
		if (collision_distance < collision.distance) {
			collision.position = collision_position;
			collision.distance = collision_distance;
			return true;
		}
	}
	return false;
}

const glm::mat4& Drawable::getToWorld() const {
	return this->toWorld;
}
