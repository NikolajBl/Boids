#include "./EntityManager.h"
#include <iostream>
#include "../Constants.h"
#include "../Collision.h"
#include "./Components/ColliderComponent.h"
#include "../lib/glm/glm.hpp"
#include <math.h>
#include "Entity.h"
#include <random>

const float maxTurnDegree = 10;
const float maxTurnRadian = glm::radians(maxTurnDegree);
const float visionRadius = 50;
const float visionDegrees = 180;
const float cohesionDegrees = 180;
const float borderCheck[9] = { -(glm::radians(150.0)) , -(glm::radians(30.0)), -(glm::radians(20.0)), -(glm::radians(10.0)), (glm::radians(0.0)), (glm::radians(10.0)), (glm::radians(20.0)), (glm::radians(30.0)), (glm::radians(150.0)) };

/*naive plan 
7 directions based on velocity. -30 -20 -10 0 10 20 30 
for all directions, check distance to border. 
if any distance is smaller than 50, pick vector with longest distance to border. 
otherwise return zero vector. 
*/


float calcDistToBorder(glm::vec2 v, glm::vec2 c) {
	if (v.x < 0 && v.y < 0) {
		float xMult = c.x / v.x;
		float xLength = length(v * xMult);
		float yMult = c.y / v.y;
		float yLength = length(v * yMult);
		//std::cout << xLength << std::endl;
		//std::cout << yLength << std::endl;
		return glm::min(xLength, yLength);
	}
	else if (v.x > 0 && v.y > 0) {
		float xMult = (WINDOW_WIDTH-c.x) / v.x;
		float xLength = length(v * xMult);
		float yMult = (WINDOW_HEIGHT-c.y) / v.y;
		float yLength = length(v * yMult);
		return glm::min(xLength, yLength);
	}
	else if (v.x < 0 && v.y > 0) {
		float xMult = c.x / v.x;
		float xLength = length(v * xMult);
		float yMult = (WINDOW_HEIGHT - c.y) / v.y;
		float yLength = length(v * yMult);
		return glm::min(xLength, yLength);
	}
	else if (v.x > 0 && v.y < 0) {
		float xMult = (WINDOW_WIDTH - c.x) / v.x;
		float xLength = length(v * xMult);
		float yMult = c.y / v.y;
		float yLength = length(v * yMult);
		return glm::min(xLength, yLength);
	}
	std::cout << "should not happen" << v.x <<  "  " << v.y << std::endl;

}

glm::vec2 EntityManager::avoidBorders(glm::vec2 velocity, glm::vec2 center) {
	if (velocity.x == 0 && velocity.y == 0) return velocity;
	std::vector<float> dists;
	bool nearBorder = false;
	int maxDistIdx;
	float maxDist = 0;
	for (int i = 0; i < 9; i++) {
		float dist = calcDistToBorder(glm::vec2(velocity.x * cos(borderCheck[i]) - velocity.y * sin(borderCheck[i]), velocity.x * sin(borderCheck[i]) + velocity.y * cos(borderCheck[i])), center);
		if (dist < 20) {
			nearBorder = true;
		}
		if (dist > maxDist) {
			maxDist = dist;
			maxDistIdx = i;
		}
		dists.push_back(dist);
	}
	if (nearBorder == true) {
		return normalize(glm::vec2(velocity.x * cos(borderCheck[maxDistIdx]) - velocity.y * sin(borderCheck[maxDistIdx]), velocity.x * sin(borderCheck[maxDistIdx]) + velocity.y * cos(borderCheck[maxDistIdx])));
	}
	//TODO change to zero vector
	return glm::vec2(0, 0);
}

inline glm::vec2 rotateWithLimit(glm::vec2 velocity, glm::vec2 targetvelocity, bool towards) {
	// not converting to degrees yet to preserve sign. 
	float radianRotation = atan2((velocity.x * targetvelocity.y - velocity.y * targetvelocity.x), (velocity.x * targetvelocity.x + velocity.y * targetvelocity.y));
	int rotation = glm::degrees(abs(radianRotation));
	if ( rotation > maxTurnDegree) {
		glm::vec2 normalVelocity = glm::normalize(velocity);
		// if the angle is positive, rotate 5 degrees in the negative direction.
		if (radianRotation >= 0.0) {
			float turnDegree = towards ? maxTurnRadian : -maxTurnRadian;
			return glm::vec2(normalVelocity.x * cos(turnDegree) - normalVelocity.y * sin(turnDegree), normalVelocity.x * sin(turnDegree) + normalVelocity.y * cos(turnDegree));
		}
		// if the angle is negative rotate 5 degrees in the positive direction.
		else {
			float turnDegree = towards ? -maxTurnRadian : maxTurnRadian;
			return glm::vec2(normalVelocity.x * (cos(turnDegree)) - normalVelocity.y * (sin(turnDegree)), normalVelocity.x * (sin(turnDegree)) + normalVelocity.y * (cos(turnDegree)));
		}
	}
	else {
		
		return towards ? targetvelocity : -targetvelocity;
	}
}


void EntityManager::clearData() {
	for (auto& entity : entities) {
		entity->destroy();
	}
}

bool EntityManager::hasNoEntities() const {
	return entities.size() == 0;
}

void EntityManager::update(float deltaTime) {
	// this is safe i think.. but i am definitely not sure. 
	for (int i = 0; i < entities.size(); i++) {
		if (!entities[i]->isActiveF()) {
			entities.erase(entities.begin() + i);
		}
		else {
			entities[i]->update(deltaTime);
		}
	}
}

void EntityManager::destroyInactiveEntities() {
	// merged with update
}

void EntityManager::render() {
	for (int layerNumber = 0; layerNumber < NUM_LAYERS; layerNumber++) {
		for (auto& entity : getEntitiesByLayer(static_cast<LayerType>(layerNumber))) {
			entity->render();
		}
	}
}

Entity& EntityManager::addEntity(std::string entityID, LayerType layer) {
	Entity* entity = new Entity(*this, entityID, layer);
	entities.emplace_back(entity);
	return *entity;
}

std::vector<Entity*> EntityManager::getEntities() const {
	return entities;
}

unsigned int EntityManager::getEntityCount() const {
	return entities.size();
}

std::vector<Entity*> EntityManager::getEntitiesByLayer(LayerType layer) const {
	std::vector<Entity*> selectedEntities;
	for (auto& entity : entities) {
		if (entity->layer == layer) {
			selectedEntities.emplace_back(entity);
		}
	}
	return selectedEntities;
}

void EntityManager::listAllEntities() const {
	for (auto& entity : entities) {
		std::cout << "Entity Name: " << entity->uid << std::endl;
		entity->listAllComponents();
		std::cout << std::endl;
	}
}


void EntityManager::simulateFlock() {
	std::vector<glm::vec2> velocities;
	for (int i = 0; i < entities.size(); i ++) {
		Entity* entity = entities[i];
		TransformComponent* transform = entity->GetComponent<TransformComponent>();
		// experimental border stuff
		glm::vec2 borderAvoid = EntityManager::avoidBorders(transform->velocity, transform->getCenter());
		if (borderAvoid.x != 0 && borderAvoid.y != 0) {
			velocities.push_back(glm::vec2(borderAvoid.x * 141.4213, borderAvoid.y * 141.4213));
		}
		else {
			glm::vec2 alignment = EntityManager::alignment(entity, i);
			glm::vec2 separation = EntityManager::separation(entity, i);
			glm::vec2 cohesion = EntityManager::cohesion(entity, i);
			glm::vec2 combined = glm::normalize(glm::vec2(alignment.x + separation.x * 1.2 + cohesion.x, alignment.y + separation.y*1.2 + cohesion.y));
			velocities.push_back(glm::vec2(combined.x * (144.0), combined.y * (144.0)));
		}
		
	}

	for (int i = 0; i < entities.size(); i++)
	{
		TransformComponent* transform = entities[i]->GetComponent<TransformComponent>();
		transform->velocity = velocities[i];
	}
}

glm::vec2 EntityManager::alignment(Entity* entity, int i) {
	TransformComponent* thisTransform = entity->GetComponent<TransformComponent>();
	glm::vec2 thisCenter = thisTransform->getCenter();
	glm::vec2 sumVel = glm::vec2(0, 0);
	glm::vec2 velocity = thisTransform->velocity;
	int count = 0;
	for (int j = 0; j < entities.size(); j++) {
		if (i != j) {
			Entity* thatEntity = entities[j];
			TransformComponent* thatTransform = thatEntity->GetComponent<TransformComponent>();
			glm::vec2 thatCenter = thatTransform->getCenter();
			glm::vec2 centerVector = glm::vec2(thatCenter.x - thisCenter.x, thatCenter.y - thisCenter.y);
			if (glm::distance(thisCenter, thatCenter) < visionRadius) {
				if (abs(glm::degrees(atan2((velocity.x * centerVector.y - velocity.y * centerVector.x), (velocity.x * centerVector.x + velocity.y * centerVector.y)))) < (visionDegrees / 2)) {
					sumVel.x += thatTransform->velocity.x;
					sumVel.y += thatTransform->velocity.y;
					count++;
				}
			}
			
		}
	}
	if (!(sumVel.x > -0.001 && sumVel.x < 0.001 && sumVel.y > -0.001 && sumVel.y < 0.001)) {
		glm::vec2 avgVel = glm::vec2(sumVel.x / count, sumVel.y / count);
		// if birds are reasonably close to the center, it doesnt matter.
		//return  glm::normalize(avgVel);
		return normalize(rotateWithLimit(velocity, glm::normalize(avgVel), true));
	}
	// return the velocity of the agent if no nearby birds. 
	return glm::normalize(velocity);
}

glm::vec2 EntityManager::cohesion(Entity* entity, int i) {
	TransformComponent* thisTransform = entity->GetComponent<TransformComponent>();
	glm::vec2 thisCenter = thisTransform->getCenter();
	glm::vec2 sumPos = glm::vec2(0, 0);
	glm::vec2 velocity = thisTransform->velocity;
	int count = 0;
	for (int j = 0; j < entities.size(); j++) {
		if (i != j) {
			Entity* thatEntity = entities[j];
			TransformComponent* thatTransform = thatEntity->GetComponent<TransformComponent>();
			glm::vec2 thatCenter = thatTransform->getCenter();
			glm::vec2 centerVector = glm::vec2(thatCenter.x - thisCenter.x, thatCenter.y - thisCenter.y);
			if (glm::distance(thisCenter, thatCenter) < visionRadius) {
				if (abs(glm::degrees(atan2((velocity.x * centerVector.y - velocity.y * centerVector.x), (velocity.x * centerVector.x + velocity.y * centerVector.y)))) <= (cohesionDegrees / 2)) {
					sumPos.x += thatCenter.x;
					sumPos.y += thatCenter.y;
					count++;
				}
			}			
			
		}
	}
	if (!(sumPos.x > -0.001 && sumPos.x < 0.001 && sumPos.y > -0.001 && sumPos.y < 0.001)) {
		glm::vec2 avgPos = glm::vec2(sumPos.x / count, sumPos.y / count);
		// if birds are reasonably close to the center, it doesnt matter.
		if (abs(glm::distance(avgPos, thisCenter)) > 1.0) {

			return normalize(rotateWithLimit(velocity, normalize(glm::vec2((avgPos.x - thisCenter.x), (avgPos.y - thisCenter.y))), true));
			//return glm::normalize(glm::vec2((avgPos.x - thisCenter.x), (avgPos.y - thisCenter.y)));
		}
	}	
	return normalize(velocity);

}

glm::vec2 EntityManager::separation(Entity* entity, int i) {

	TransformComponent* thisTransform = entity->GetComponent<TransformComponent>();
	glm::vec2 thisCenter = thisTransform->getCenter();
	glm::vec2 distVect = glm::vec2(0, 0);
	glm::vec2 velocity = thisTransform->velocity;
	for (int j = 0; j < entities.size(); j++) {
		if (i != j) {
			Entity* thatEntity = entities[j];
			TransformComponent* thatTransform = thatEntity->GetComponent<TransformComponent>();
			glm::vec2 thatCenter = thatTransform->getCenter();
			glm::vec2 centerVector = glm::vec2(thatCenter.x - thisCenter.x, thatCenter.y - thisCenter.y);
			if (glm::distance(thisCenter, thatCenter) < visionRadius) {
				if (abs(glm::degrees(atan2((velocity.x * centerVector.y - velocity.y * centerVector.x), (velocity.x * centerVector.x + velocity.y * centerVector.y)))) < (visionDegrees / 2)) {
					distVect.x += (thatCenter.x - thisCenter.x);
					distVect.y += (thatCenter.y - thisCenter.y);
				}
			}
			
		}
	}
	if (!(distVect.x > -0.001 && distVect.x < 0.001 && distVect.y > -0.001 && distVect.y < 0.001)) {
		return glm::normalize(rotateWithLimit(velocity, glm::normalize(distVect), false));
	}
	else {
		return glm::normalize(velocity);
	}

}

// this is very ugly and inefficient.. but it's fine for my ambitions with this project. 
CollisionType EntityManager::checkCollisions() const {
	for (int i = 0; i < entities.size(); i++) {
		Entity* thisEntity = entities[i];
		if (thisEntity->HasComponent<ColliderComponent>()) {
			ColliderComponent* thisCollider = thisEntity->GetComponent<ColliderComponent>();
			for (int j = i+1; j < entities.size(); j++) {
				Entity* thatEntity = entities[j];
				if (thatEntity->HasComponent<ColliderComponent>() && (thisEntity->uid.compare(thatEntity->uid) != 0)) {
					ColliderComponent* thatCollider = thatEntity->GetComponent<ColliderComponent>();
					if (Collision::checkRectangleCollision(thisCollider->collider, thatCollider->collider)) {
						if (thisCollider->colliderTag.compare("player") == 0 && thatCollider->colliderTag.compare("enemy") == 0) {
							return PLAYER_ENEMY_COLLISION;
						}
						if (thisCollider->colliderTag.compare("player") == 0 && thatCollider->colliderTag.compare("projectile") == 0) {
							return PLAYER_PROJECTILE_COLLISION;
						}
						if (thisCollider->colliderTag.compare("enemy") == 0 && thatCollider->colliderTag.compare("friendly_projectile") == 0) {
							return ENEMY_PROJECTILE_COLLISION;
						}
						if (thisCollider->colliderTag.compare("player") == 0 && thatCollider->colliderTag.compare("level_complete") == 0) {
							return PLAYER_LEVEL_COMPLETE_COLLISION;
						}
					}
				}
			}
		}
	}
}