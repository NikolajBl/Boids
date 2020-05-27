#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Entity.h"
#include <vector>
#include <string>
#include "../Constants.h"
#include "glm/glm.hpp"

class Entity;

class EntityManager {
	private: 
		std::vector<Entity*> entities;
	public:
		void clearData();
		void render();
		void update(float deltaTime);
		bool hasNoEntities() const;
		Entity& addEntity(std::string entityName, LayerType layer);
		std::vector<Entity*> getEntitiesByLayer(LayerType layer) const;
		std::vector<Entity*> getEntities() const;
		unsigned int getEntityCount() const;
		void listAllEntities() const;
		CollisionType checkCollisions() const;
		glm::vec2 separation(Entity* entity, int i);
		glm::vec2 cohesion(Entity* entity, int i);
		glm::vec2 alignment(Entity* entity, int i);
		glm::vec2 avoidBorders(glm::vec2 velocity, glm::vec2 center);
		void simulateFlock();
		void destroyInactiveEntities();


};

#endif