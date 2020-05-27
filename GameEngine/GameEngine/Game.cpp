#include <iostream>
#include "Constants.h"
#include "Game.h"
#include "./lib/glm/glm.hpp"
#include "Entity-Component/Components/TransformComponent.h"
#include "./Entity-Component/Components/SpriteComponent.h"
#include "./Entity-Component/Components/KeyboardControlComponent.h"
#include "./Entity-Component/Components/ColliderComponent.h"
#include "./Entity-Component/Components/TextLabelComponent.h"
#include "./Entity-Component/Components/ProjectileEmitterComponent.h"
#include "./Managers/AssetManager.h"
#include "./Managers/FlockSimulationManager.h"
#include "Map.h"
#include <SDL.h>

EntityManager manager;
AssetManager* Game::assetManager = new AssetManager(&manager);
SDL_Renderer* Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map* map;


Game::Game() : ticksLastFrame(0) {
	this->isRunning = false;
}

Game::~Game() {

}

bool Game::isRunningF() const {
	return this->isRunning;
}


void Game::initialize(int width, int height) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL." << std::endl;
		return;
	}
	if (TTF_Init() != 0) {
		std::cerr << "Error initializing SDL TTF." << std::endl;
		return;
	}

	window = SDL_CreateWindow(
		NULL,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		SDL_WINDOW_BORDERLESS
	);

	if (!window) {
		std::cerr << "Error creating DSL window." << std::endl;
		return;
	}
	Game::renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer) {
		std::cerr << "Error creating SDL renderer." << std::endl;
	}

	loadLevel(0);

	isRunning = true;
	return;
}

//Entity& player(manager.addEntity("chopper", PLAYER_LAYER));

void Game::loadLevel(int levelNumber) {
	/* Add assets to the assetmanager list */

	assetManager->addTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
	assetManager->addTexture("bird-image", std::string("./assets/images/bird.png").c_str());
	assetManager->addTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
	assetManager->addTexture("radar-image", std::string("./assets/images/radar.png").c_str());
	assetManager->addTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
	assetManager->addTexture("boundingbox-texture", std::string("./assets/images/collision-16x16-texture.png").c_str());
	assetManager->addTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
	assetManager->addTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());
	assetManager->addFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), /*fontsize*/ 14);

	//map = new Map("jungle-tiletexture", mapScale, 32);

	//map->loadMap("./assets/tilemaps/jungle.map", 25, 20);

	/* Load entities and add components to them */
	//Entity& tankEntity(manager.addEntity("tank", ENEMY_LAYER));
	//tankEntity.AddComponent<TransformComponent>(150, 495, 5, 0, 32, 32, 1);
	//tankEntity.AddComponent<SpriteComponent>("tank-image");
	//tankEntity.AddComponent<ColliderComponent>("enemy", 240, 106, 32, 32);

	//Entity& projectile(manager.addEntity("projectile", PROJECTILE_LAYER));
	//projectile.AddComponent<TransformComponent>(166, 511, 0, 0, 4, 4, 1);
	//projectile.AddComponent<SpriteComponent>("projectile-image");
	//projectile.AddComponent<ColliderComponent>("projectile", 166, 511, 4, 4);
	//projectile.AddComponent<ProjectileEmitterComponent>(/*speed*/ 50, /*angel*/ 270, /*range*/ 200, true);

	//Entity& heliport(manager.addEntity("heliport", VEGETATION_LAYER)); // TODO maybe add another layer
	//heliport.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
	//heliport.AddComponent<SpriteComponent>("heliport-image");
	//heliport.AddComponent<ColliderComponent>("level_complete", 470, 420, 32, 32);
	FlockSimulationManager::generateBirds();



}

void Game::processInput() {
	SDL_PollEvent(&event);
	switch (event.type) {
		case SDL_QUIT: {
			isRunning = false;
			break;
		}
		case SDL_KEYDOWN: {
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				isRunning = false;
			}
			break;
		}
		default: {
			break;
		}
	}
}

void Game::update() {
	// Wait until 16 ms has ellapsed since the last frame 
	int waitTime = (ticksLastFrame + FRAME_TARGET_TIME) - SDL_GetTicks();
	if (waitTime > 0 && waitTime < FRAME_TARGET_TIME) {
		SDL_Delay(waitTime);
	}
	// Alternative approach (takes up all process power though and thus is unpractical in reality.)
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksLastFrame + FRAME_TARGET_TIME));

	// Delta time is the difference in ticks between last frame and now. 
	float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

	// limit the maximum deltatime (to avoid huge jumps between frames)
	deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;
	
	// Sets the new ticks for the current frame to be used in the next pass
	ticksLastFrame = SDL_GetTicks();

	manager.update(deltaTime);

	//handleCameraMovement();
	//checkCollisions();
	manager.simulateFlock();
}

void Game::render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	
	// clear the back buffer
	SDL_RenderClear(renderer);

	// draw objects on the scene (back buffer)

	if (manager.hasNoEntities()) {
		return; 
	}

	manager.render();
	

	// swap the front and back buffer
	SDL_RenderPresent(renderer);
}

void Game::nextLevel(int levelNumber) {
	std::cout << "Next level" << std::endl;
	isRunning = false;
}

void Game::gameOver() {
	std::cout << "Game over" << std::endl;
	isRunning = false;
}

void Game::destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}



void Game::showContent() const {
	manager.listAllEntities();
}


