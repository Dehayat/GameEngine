#pragma once
#include <string>

struct SpriteComponent {
	std::string assetId;
	int height;
	int width;
	int zIndex;
	SDL_Rect srcRect;
	bool isFixed;

	SpriteComponent(std::string assetId = "", int width = 0, int height = 0, int zIndex = 1, int offsetX = 0, int offsetY = 0) {
		this->assetId = assetId;
		this->width = width;
		this->height = height;
		this->zIndex = zIndex;
		srcRect = {
			offsetX,
			offsetY,
			width,
			height
		};
		isFixed = false;
	}
};