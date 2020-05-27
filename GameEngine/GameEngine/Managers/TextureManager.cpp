#include "./TextureManager.h"

SDL_Texture* TextureManager::loadTexture(const char* fileName) {
	SDL_Surface* surface = IMG_Load(fileName);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
	SDL_FreeSurface(surface);
	return texture;
}

void TextureManager::draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect targetRectangle, SDL_RendererFlip flip) {
	SDL_Point center = { (targetRectangle.x + (targetRectangle.w / 2)),
						 (targetRectangle.y + (targetRectangle.h / 2)) };
	SDL_RenderCopyEx(Game::renderer, texture, &sourceRectangle, &targetRectangle, 0.0, &center, flip);
}

void TextureManager::drawWithAngle(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect targetRectangle, SDL_RendererFlip flip, unsigned int angle) {
	// center argument is set to NULL because we want to rotate around the center of the target rectangle. 
	SDL_RenderCopyEx(Game::renderer, texture, &sourceRectangle, &targetRectangle, angle, NULL, flip);
}
