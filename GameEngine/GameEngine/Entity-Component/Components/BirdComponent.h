#ifndef BIRDCOMPONENT_H
#define BIRDCOMPONENT_H

#include "../Entity-Component/Component.h"
#include "./TransformComponent.h"
#include "./ColliderComponent.h"

class BirdComponent : public Component {
public:
	ColliderComponent* collider;
	TransformComponent* transform;

	BirdComponent() {

	}

	void update(float deltaTime) override {

	}

	void initialize() override[
		if (owner->HasComponent<TransformComponent>()) {
			transform = owner->GetComponent<TransformComponent>();
		}
		if (owner->HasComponent<ColliderComponent>()) {
			collider = owner->GetComponent<ColliderComponent>();
		}
	]

};

#endif