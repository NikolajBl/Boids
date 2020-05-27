#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>
#include "./Component.h"
#include "./EntityManager.h"
#include "../Constants.h"

class Component;
class EntityManager;


class Entity {
private:
	EntityManager& manager;
	bool isActive;
	std::vector<Component*> components;
	std::map<const type_info*, Component*> componentTypeMap;
public:
	std::string uid;
	LayerType layer;
	Entity(EntityManager& manager);
	Entity(EntityManager& manager, std::string uid, LayerType layer);
	void update(float deltaTime);
	void render();
	void destroy();
	bool isActiveF() const; // const prevents the function from changing the object from which they are called. 
	void listAllComponents() const;

	// Template. Allows me to generically add new components to my entity. 
	template <typename T, typename... TArgs>
	T& AddComponent(TArgs&&... args) {
		T* newComponent(new T(std::forward<TArgs>(args)...));
		newComponent->owner = this;
		components.emplace_back(newComponent);
		componentTypeMap[&typeid(*newComponent)] = newComponent;
		newComponent->initialize();
		return *newComponent;
	}

	template <typename T>
	T* GetComponent() {
		return static_cast<T*>(componentTypeMap[&typeid(T)]);
	}

	template <typename T>
	bool HasComponent() const {
		return componentTypeMap.count(&typeid(T));

	}


};

#endif