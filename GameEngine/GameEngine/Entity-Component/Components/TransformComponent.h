#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include "../EntityManager.h"
#include "../../lib/glm/glm.hpp"
#include "../../Game.h"
#include <iostream>
#include <SDL.h>
#include "../Constants.h"
#include <random>

class TransformComponent : public Component {
public: 
	glm::vec2 position;
	glm::vec2 velocity;
	int speed;
	int width;
	int height;
	int scale;

	TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s) {
		position = glm::vec2(posX, posY);
		velocity = glm::vec2(velX, velY);
		width = w;
		height = h;
		scale = s;
	}

	glm::vec2 getCenter() {
		return glm::vec2(this->position.x + (this->width / 2), this->position.y + (this->height / 2));
	}

	void initialize() override {
		speed = (rand() % 100) + 100;
		return;
	}

	void update(float deltaTime) override {
		// This is where the position of the "entity" is updated with respect to the time that has passed. 
		// Check if entities are leaving the screen. If they are, teleport them to the other side of the map.


		if (position.x < 0 && velocity.x < 0) {
			position.x = WINDOW_WIDTH*mapScale;
		}
		else if (position.x > WINDOW_WIDTH*mapScale && velocity.x > 0) {
			position.x = 0;
		}
		else if (position.y > WINDOW_HEIGHT*mapScale && velocity.y > 0) {
			position.y = 0;
		}
		else if (position.y < 0 && velocity.y < 0) {
			position.y = WINDOW_HEIGHT*mapScale;
		}
		else {
			position.x += velocity.x * deltaTime;
			position.y += velocity.y * deltaTime;
		}
	}

	void render() override {

	}

	std::string toString() const override {
		return "Component<TransformComponent";
	}

};

#endif