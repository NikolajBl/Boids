#ifndef FLOCKSIMULATIONMANAGER_H
#define FLOCKSIMULATIONMANAGER_H
#include "../Entity-Component/EntityManager.h"
#include "./TextureManager.h"
#include "../Entity-Component/Components/ColliderComponent.h"
#include "../Entity-Component/Components/KeyboardControlComponent.h"
#include "../Entity-Component/Components/ProjectileEmitterComponent.h"
#include "../Entity-Component/Components/SpriteComponent.h"
#include "../Entity-Component/Components/TransformComponent.h"




class FlockSimulationManager {

public: 
	static void generateBirds();

};

#endif