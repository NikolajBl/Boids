#ifndef COMPONENT_H
#define COMPONENT_H

#include "Entity.h"
class Entity;

class Component {
public:
	Entity* owner;
	virtual ~Component() {}
	virtual void initialize() {}
	virtual void update(float deltaTime) {}
	virtual void render() {}
	virtual std::string toString() const { return "Undefined Component"; }
};


#endif