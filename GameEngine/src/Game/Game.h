#pragma once
#include <SDL.h>
#include "../ECS/ECS.h"
#include "../AssetStore/AssetStore.h"
#include "../EventBus/EventBus.h"

const int Fps = 30;
const int MillisecondsPerFrame = 1000 / Fps;

class Game
{
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool isRunning;
	int lastFrameStartInMilliseconds;
	std::unique_ptr<Registry> registry;
	std::unique_ptr<AssetStore> assetStore;
	std::unique_ptr<EventBus> eventBus;
	SDL_Rect camera;

public:
	Game();
	~Game();
	void Init();
	void Run();
	void LoadTileMap();
	void LoadLevel(int level);
	void Setup();
	void HandleInput();
	void Update();
	void Render();
	void Destroy();

	int windowWidth;
	int windowHeight;
};