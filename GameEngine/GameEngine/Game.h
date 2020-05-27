#ifndef GAME_H
#define GAME_H

#include<SDL.h>
#include<SDL_image.h>
#include <SDL_ttf.h>
#include "Entity-Component/EntityManager.h"
#include "Entity-Component/Component.h"
#include "Entity-Component/Entity.h"
#undef main

class AssetManager;

class Game {
private:
	bool isRunning;
	SDL_Window* window;
public:
	Game();
	~Game();
	static SDL_Renderer* renderer;
	static AssetManager* assetManager;
	static SDL_Event event;
	static SDL_Rect camera;
	int ticksLastFrame;
	bool isRunningF() const;
	void loadLevel(int levelNumber);
	void initialize(int width, int height);
	void processInput();
	void update();
	void render();
	void destroy();
	void showContent() const;
	void handleCameraMovement();
	void checkCollisions();
	void gameOver();
	void nextLevel(int levelNumber);

};

#endif // !"GAME_H"

