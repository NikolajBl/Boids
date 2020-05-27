
#include "FlockSimulationManager.h"
#include "../Entity-Component/Entity.h"
#include <random>

extern EntityManager manager;
const int flockspeed = 100;

void FlockSimulationManager::generateBirds() {
	//positive x y velocities
	for (int i = 0; i < 30; i++) {
		int x = rand() % 800;
		int y = rand() % 600;
		Entity& bird(manager.addEntity("bird-image", ENEMY_LAYER));
		bird.AddComponent<TransformComponent>(x, y, 100, 100, 16, 16, 1);
		bird.AddComponent<SpriteComponent>("bird-image", true);
	}
	//negative x positive y velocities
	for (int i = 0; i < 30; i++) {
		int x = rand() % 800;
		int y = rand() % 600;
		Entity& bird(manager.addEntity("bird-image", ENEMY_LAYER));
		bird.AddComponent<TransformComponent>(x, y, -100, 100, 16, 16, 1);
		bird.AddComponent<SpriteComponent>("bird-image", true);
	}
	//positive x negative y velocities
	for (int i = 0; i < 30; i++) {
		int x = rand() % 800;
		int y = rand() % 600;
		Entity& bird(manager.addEntity("bird-image", ENEMY_LAYER));
		bird.AddComponent<TransformComponent>(x, y, 100, -100, 16, 16, 1);
		bird.AddComponent<SpriteComponent>("bird-image", true);
	}
	// negative x y velocities
	for (int i = 0; i < 30; i++) {
		int x = rand() % 800;
		int y = rand() % 600;
		Entity& bird(manager.addEntity("bird-image", ENEMY_LAYER));
		bird.AddComponent<TransformComponent>(x, y, -100, -100, 16, 16, 1);
		bird.AddComponent<SpriteComponent>("bird-image", true);
	}
}