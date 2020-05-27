#include <iostream>
#include "./Constants.h"
#include "./Game.h""

int main() {
	Game* game = new Game();
	game->initialize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//game->showContent();

	while (game->isRunningF()) {
		game->processInput();
		game->update();
		game->render();
	}

	game->destroy();




	return 0;
}