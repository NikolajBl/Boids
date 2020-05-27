#ifndef COLLIDERCOMPONENT_H
#define COLLIDERCOMPONENT_H

#include "../Game.h"
#include "../EntityManager.h"
#include "./TransformComponent.h"
#include <SDL.h>
#include "../Managers/TextureManager.h"
#include "../Managers/AssetManager.h"

class ColliderComponent : public Component {
public:
	std::string colliderTag;
	SDL_Rect collider;
	SDL_Rect sourceRectangle;
	SDL_Rect targetRectangle;
	SDL_Texture *texture;
	TransformComponent* transform;

	ColliderComponent(std::string colliderTag, int x, int y, int width, int height) {
		this->colliderTag = colliderTag;
		this->collider = { x, y, width, height };
	}

	void initialize() override {
		//only for testing bounding boxes. 
		setTexture("boundingbox-texture");
		if (owner->HasComponent<TransformComponent>()) {
			transform = owner->GetComponent<TransformComponent>();
			sourceRectangle = { 0, 0, transform->width, transform->height };
			targetRectangle = { collider.x, collider.y, collider.w * transform->scale, collider.h * transform->scale };
		}
	}

	void update(float deltaTime) override {
		collider.x = static_cast<int>(transform->position.x);
		collider.y = static_cast<int>(transform->position.y);
		collider.w = transform->width * transform->scale;
		collider.h = transform->height * transform->scale;
		targetRectangle.x = collider.x - Game::camera.x;
		targetRectangle.y = collider.y - Game::camera.y;

	}

	void render() override {
		// only for testing bounding boxes
		TextureManager::draw(texture, sourceRectangle, targetRectangle, SDL_FLIP_NONE);
	}

	void setTexture(std::string assetTextureId) {
		texture = Game::assetManager->getTexture(assetTextureId);
	}
};

#endif