#include "Game.h"
#include <iostream>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/SpriteComponent.h"
#include "../Components/AnimationComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/KeyboardMovementComponent.h"
#include "../Systems/MovementSystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/CollisionSystem.h"
#include "../Systems/RenderColliderSystem.h"
#include "../Systems/DamageSystem.h"
#include "../Systems/KeyboardMovementSystem.h"

Game::Game()
{
	renderer = nullptr;
	window = nullptr;
	lastFrameStartInMilliseconds = 0;
	isRunning = false;
	windowWidth = 100;
	windowHeight = 100;
	registry = std::make_unique<Registry>();
	assetStore = std::make_unique<AssetStore>();
	eventBus = std::make_unique<EventBus>();
}

Game::~Game()
{}

void Game::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		Logger::Err("Error in init SDL");
	}
	else
	{
		SDL_DisplayMode displayMode;
		SDL_GetCurrentDisplayMode(0, &displayMode);
		windowWidth = displayMode.w;
		windowHeight = displayMode.h;
		window = SDL_CreateWindow(
			"Game Engine",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			windowWidth,
			windowHeight,
			//SDL_WINDOW_FULLSCREEN_DESKTOP
			//SDL_WINDOW_FULLSCREEN
			SDL_WINDOW_BORDERLESS
		);
		if (window == nullptr)
		{
			Logger::Err("Error Could not SDL create window");
		}
		else
		{
			renderer = SDL_CreateRenderer(window, -1, 0);
			if (renderer == nullptr)
			{
				Logger::Err("Error Could not SDL renderer create" + 10);
			}
			else
			{
				isRunning = true;
			}
		}
	}
}

void Game::Run()
{
	Setup();
	while (isRunning)
	{
		HandleInput();
		Update();
		Render();
	}
}

glm::vec2 playerPos;
glm::vec2 playerVel;


void Game::LoadTileMap() {
	const int tileWidth = 32;
	const int tileHeight = 32;
	const int tileMapWidth = 10;
	const int tileMapHeight = 3;
	int currentRow = 0;
	int currentCol = 0;
	bool waitForNumber = true;
	std::ifstream inputStream("./assets/tilemaps/jungle.map");
	while (inputStream.peek() != EOF) {
		if (waitForNumber) {
			int tileIndex;
			inputStream >> tileIndex;

			int tileOffsetCol = tileIndex % tileMapWidth;
			int tileOffsetRow = tileIndex / tileMapWidth;

			Entity tileEntity = registry->CreateEntity();
			tileEntity.AddComponent<TransformComponent>(glm::vec2(tileWidth * currentCol, tileHeight * currentRow));
			tileEntity.AddComponent<SpriteComponent>("JungleTiles", tileWidth, tileHeight, 0, tileOffsetCol * tileWidth, tileOffsetRow * tileHeight);
			waitForNumber = false;
		}
		else {
			char symbol;
			inputStream >> symbol;
			if (symbol == ',') {
				currentCol++;
			}
			else {
				currentRow++;
				currentCol = 0;
			}
			waitForNumber = true;
		}
	}

	Logger::Log("Tile Map Created");
}

void Game::LoadLevel(int level) {
	registry->AddSystem<MovementSystem>();
	registry->AddSystem<RenderSystem>();
	registry->AddSystem<AnimationSystem>();
	registry->AddSystem<CollisionSystem>();
	registry->AddSystem<RenderColliderSystem>();
	registry->AddSystem<DamageSystem>();
	registry->AddSystem<KeyboardMovementSystem>();

	assetStore->AddTexture(renderer, "TankImage", "./assets/images/tank-panther-right.png");
	assetStore->AddTexture(renderer, "TruckImage", "./assets/images/truck-ford-right.png");
	assetStore->AddTexture(renderer, "JungleTiles", "./assets/tilemaps/jungle.png");
	assetStore->AddTexture(renderer, "ChopperImage", "./assets/images/chopper-spritesheet.png");
	assetStore->AddTexture(renderer, "RadarImage", "./assets/images/radar.png");

	LoadTileMap();


	Entity chopper = registry->CreateEntity();
	chopper.AddComponent<TransformComponent>(glm::vec2(150, 150));
	chopper.AddComponent<RigidBodyComponent>(glm::vec2(0, 0));
	chopper.AddComponent<SpriteComponent>("ChopperImage", 32, 32, 3);
	chopper.AddComponent<AnimationComponent>(2, 10, true);
	chopper.AddComponent<KeyboardMovementComponent>(50.0f);

	Entity radar = registry->CreateEntity();
	radar.AddComponent<TransformComponent>(glm::vec2(300, 20));
	radar.AddComponent<SpriteComponent>("RadarImage", 64, 64, 4);
	radar.AddComponent<AnimationComponent>(8, 4, true);

	Entity tank = registry->CreateEntity();
	tank.AddComponent<TransformComponent>(glm::vec2(100, 20));
	tank.AddComponent<RigidBodyComponent>(glm::vec2(10, 0));
	tank.AddComponent<SpriteComponent>("TankImage", 32, 32, 2);
	tank.AddComponent<BoxColliderComponent>(20, 18, glm::vec2(6, 7));

	Entity truck = registry->CreateEntity();
	truck.AddComponent<TransformComponent>(glm::vec2(10, 20));
	truck.AddComponent<RigidBodyComponent>(glm::vec2(30, 0));
	truck.AddComponent<SpriteComponent>("TruckImage", 32, 32, 2);
	truck.AddComponent<BoxColliderComponent>(24, 22, glm::vec2(4, 5));
}

void Game::Setup()
{
	LoadLevel(1);
}

void Game::HandleInput()
{
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
		{
			isRunning = false;
			break;
		}
		case SDL_KEYDOWN:
		{
			if (sdlEvent.key.keysym.sym == SDLK_ESCAPE)
			{
				isRunning = false;
			}
			else {
				eventBus->EmitEvent<KeyPressedEvent>(sdlEvent.key.keysym.sym);
			}
			break;
		}
		default:
		{
			break;
		}
		}
	}
}

void Game::Update()
{
	//while(!SDL_TICKS_PASSED(SDL_GetTicks(),lastFrameStartInMilliseconds+MillisecondsPerFrame));
	lastFrameStartInMilliseconds = SDL_GetTicks();
	int msToWait = lastFrameStartInMilliseconds + MillisecondsPerFrame - SDL_GetTicks();
	if (msToWait > 0)
	{
		SDL_Delay(msToWait);
	}
	double deltaTime = (SDL_GetTicks() - lastFrameStartInMilliseconds) / 1000.0f;

	//playerPos.x += playerVel.x * deltaTime;
	//playerPos.y += playerVel.y * deltaTime;

	eventBus->Reset();

	registry->GetSystem<DamageSystem>().ListenToEvents(eventBus);
	registry->GetSystem<KeyboardMovementSystem>().ListenToEvents(eventBus);

	registry->GetSystem<MovementSystem>().Update(deltaTime);
	registry->GetSystem<CollisionSystem>().Update(eventBus);
	registry->GetSystem<AnimationSystem>().Update(deltaTime);

	registry->Update();
}

void Game::Render()
{
	SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
	SDL_RenderClear(renderer);

	registry->GetSystem<RenderSystem>().Update(renderer, assetStore);
	registry->GetSystem<RenderColliderSystem>().Update(renderer);


	//SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-down.png");
	//SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	//SDL_FreeSurface(surface);
	//SDL_Rect rect = {
	//	static_cast<int>(playerPos.x),
	//	static_cast<int>(playerPos.y),
	//	100,
	//	50 };
	//SDL_RenderCopy(renderer, texture, nullptr, &rect);
	//SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}

void Game::Destroy()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	Logger::Log("Quitting");
}
