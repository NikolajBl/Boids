#ifndef TEXTLABELCOMPONENT_H
#define TEXTLABELCOMPONENT_H

#include "../EntityManager.h"
#include "../../Managers/AssetManager.h"
#include "../../Game.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include "../../Managers/FontManager.h"

class TextLabelComponent : public Component {
private:
	SDL_Rect position;
	std::string text;
	std::string fontFamily;
	SDL_Color color;
	SDL_Texture* texture;
public:
	TextLabelComponent(int x, int y, std::string text, std::string fontFamily, const SDL_Color color) {
		this->position.x = x;
		this->position.y = y;
		this->text = text;
		this->fontFamily = fontFamily;
		this->color = color;
		setLabelText(text, fontFamily);
	}

	void setLabelText(std::string text, std::string fontFamily) {
		SDL_Surface* surface = TTF_RenderText_Blended(Game::assetManager->getFont(fontFamily), text.c_str(), color);
		texture = SDL_CreateTextureFromSurface(Game::renderer, surface);
		SDL_FreeSurface(surface);
		SDL_QueryTexture(texture, NULL, NULL, &position.w, &position.h);
	}

	void render() override {
		FontManager::draw(texture, position);
	}
};

#endif