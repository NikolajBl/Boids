#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "../Game.h"

class TextureManager {
private:
	
public:
	static  SDL_Texture* loadTexture(const char* fileName);
	static void draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect targetRectangle, SDL_RendererFlip flip);
	static void drawWithAngle(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect targetRectangle, SDL_RendererFlip flip, unsigned int angle);


};


#endif