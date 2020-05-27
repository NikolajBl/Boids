#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H

#include <SDL.h>
#include <../Managers/TextureManager.h>
#include <../Managers/AssetManager.h>
#include <iostream>
#include "glm/glm.hpp"
#include "TransformComponent.h"
#include "./Animation.h"
#include <math.h>

class SpriteComponent : public Component {
private: 
	TransformComponent* transform;
	SDL_Texture* texture;
	SDL_Rect sourceRectangle;
	SDL_Rect targetRectangle;
	bool isAnimated;
	int numFrames;
	int animationSpeed;
	bool isFixed;
	bool smoothDirection;
	std::map<std::string, Animation> animations;
	std::string currentAnimationName;
	unsigned int animationIndex = 0;

public:
	const static SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
	SpriteComponent(std::string filePath, bool smoothDirection) {
		this->isAnimated = false;
		this->isFixed = false;
		this->smoothDirection = smoothDirection;

		setTexture(filePath);

	}

	SpriteComponent(std::string filePath, int numFrames, int animationSpeed, bool hasDirections, bool isFixed, bool smoothDirection) {
		this->isAnimated = true;
		this->numFrames = numFrames;
		this->animationSpeed = animationSpeed;
		this->isFixed = isFixed;
		this->smoothDirection = smoothDirection;

		if (hasDirections) {
			Animation downAnimation = Animation(0, numFrames, animationSpeed);
			Animation rightAnimation = Animation(1, numFrames, animationSpeed);
			Animation leftAnimation = Animation(2, numFrames, animationSpeed);
			Animation upAnimation = Animation(3, numFrames, animationSpeed);

			animations.emplace("downAnimation", downAnimation);
			animations.emplace("rightAnimation", rightAnimation);
			animations.emplace("leftAnimation", leftAnimation);
			animations.emplace("upAnimation", upAnimation);

			this->animationIndex = 0;
			this->currentAnimationName = "downAnimation";
		}
		else {
			Animation singleAnimation = Animation(0, numFrames, animationSpeed);
			animations.emplace("singleAnimation", singleAnimation);
			this->animationIndex = 0;
			this->currentAnimationName = "singleAnimation";
		}
		play(this->currentAnimationName);
		setTexture(filePath);

	}

	void play(std::string animationName) {
		if (this->isAnimated) {
			numFrames = animations[animationName].numFrames;
			animationIndex = animations[animationName].index;
			animationSpeed = animations[animationName].animationSpeed;
			currentAnimationName = animationName;
		}
	}

	void setTexture(std::string assetTextureId) {
		texture = Game::assetManager->getTexture(assetTextureId);
	}

	void initialize() override {
		transform = owner->GetComponent<TransformComponent>();
		sourceRectangle.x = 0;
		sourceRectangle.y = 0;
		sourceRectangle.w = transform->width;
		sourceRectangle.h = transform->height;
	}

	std::string toString() const override {
		return "Component<SpriteComponent>";
	}

	void update(float deltaTime) override {
		if (isAnimated) {
			// This thing is responsible for selecting the correct animation for the sprite. %numframes is used to create a circle. E.g. never surpass numFrames. 
			sourceRectangle.x = sourceRectangle.w * static_cast<int>((SDL_GetTicks() / animationSpeed) % numFrames);
		}
		// this thing is responsible for selecting the correct type of animation. For example if the animation supports different directions.
		sourceRectangle.y = animationIndex * transform->height;

		// is fixed is used to hold entities in place no matter the camera movement. For instance UI elements.
		targetRectangle.x = static_cast<int>(transform->position.x) - (isFixed ? 0 : Game::camera.x);
		targetRectangle.y = static_cast<int>(transform->position.y) - (isFixed ? 0 : Game::camera.y);
		targetRectangle.w = transform->width * transform->scale;
		targetRectangle.h = transform->height * transform->scale;
	}

	void render() override{
		if (smoothDirection) {
			int x = transform->velocity.x;
			int y = transform->velocity.y;
			//magic..
			int angle = glm::degrees(atan2(y, x));
			//std::cout << angle << std::endl;
			// to take care of negative angles
			angle = angle < 0 ? 360 + angle : angle;
			TextureManager::drawWithAngle(texture, sourceRectangle, targetRectangle, spriteFlip, angle);
		}
		else {
			TextureManager::draw(texture, sourceRectangle, targetRectangle, spriteFlip);
		}
	}
};

#endif