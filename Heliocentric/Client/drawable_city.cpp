#include "drawable_city.h"
#include "drawable_planet.h"
#include "sphere_model.h"
#include "drawable_slot.h"
#include "gui.h"
#include "client.h"

#include <glm/gtx/transform.hpp>

DrawableCity::DrawableCity(const City& city, Shader * shader, PlayerIcon * icon) : City(city) {
	this->model = Model::getInstance("Models/city.obj");
	this->drawable_slot = static_cast<DrawableSlot*>(this->get_slot());
	this->shader = shader;

	this->toWorld = this->drawable_slot->getToWorld();
	this->icon = icon;
}

void DrawableCity::update() {
	this->toWorld = this->drawable_slot->getToWorld();
}

void DrawableCity::draw(const Camera & camera) const {
	PlayerColor::Color player_color = this->get_player()->getColor();
	icon->setColor(glm::vec4(PlayerColor::colorToRGBVec(player_color), 1.0f));
	icon->setSize(0.02);
	icon->update(toWorld[3]);
	icon->draw(camera);

	shader->bind();
	GLuint shaderID = shader->getPid();

	//color cities based on player color
	switch (player_color) {
	case PlayerColor::WHITE:
		glUniform3f(glGetUniformLocation(shaderID, "m_color"), 1.0f, 1.0f, 1.0f);
		break;
	case PlayerColor::RED:
		glUniform3f(glGetUniformLocation(shaderID, "m_color"), 1.0f, 0.0f, 0.0f);
		break;
	case PlayerColor::BLUE:
		glUniform3f(glGetUniformLocation(shaderID, "m_color"), 0.0f, 0.0f, 1.0f);
		break;
	case PlayerColor::GREEN:
		glUniform3f(glGetUniformLocation(shaderID, "m_color"), 0.0f, 1.0f, 0.0f);
		break;
	case PlayerColor::YELLOW:
		glUniform3f(glGetUniformLocation(shaderID, "m_color"), 1.0f, 1.0f, 0.0f);
		break;
	case PlayerColor::ORANGE:
		glUniform3f(glGetUniformLocation(shaderID, "m_color"), 1.0f, 0.30f, 0.0f);
		break;
	default:
		glUniform3f(glGetUniformLocation(shaderID, "m_color"), 1.0f, 1.0f, 1.0f);
		break;
	}

	Drawable::draw(camera);
}

void DrawableCity::select(GUI* gui, Client* client) {
	gui->displayCityUI(this, std::bind(&Client::createUnitFromCity, client, this, std::placeholders::_1));
}

void DrawableCity::unselect(GUI* gui, Client* client) {
	gui->hideCityUI();
}

