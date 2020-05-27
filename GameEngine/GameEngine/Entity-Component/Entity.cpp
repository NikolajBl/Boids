#include "./Entity.h"
#include "./Component.h"
#include <iostream>

Entity::Entity(EntityManager& manager) : manager(manager) {
	this->isActive = true;
}

Entity::Entity(EntityManager& manager, std::string uid, LayerType layer) : manager(manager), uid(uid), layer(layer) { // smart way to initialize reference members of Entity object. 
	this->isActive = true;
}

void Entity::update(float deltaTime) {
	for (auto& component : components) {
		component->update(deltaTime);
	}
}

void Entity::render() {
	for (auto& component : components) {
		component->render();
	}
}

void Entity::destroy() {
	this->isActive = false;
}

bool Entity::isActiveF() const {
	return this->isActive;
}

void Entity::listAllComponents() const {
	for (auto& component : this->components) {
		std::cout << "   " << component->toString() << std::endl;
	}
}