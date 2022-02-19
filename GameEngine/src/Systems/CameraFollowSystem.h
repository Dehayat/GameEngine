#pragma once
#include "../ECS/ECS.h"
#include "../Logger/Logger.h"
#include "../Components/TransformComponent.h"
#include "../Components/CameraFollowComponent.h"
#include <SDL.h>

class CameraFollowSystem :public System {
public:
	CameraFollowSystem() {
		RequireComponent<CameraFollowComponent>();
		RequireComponent<TransformComponent>();
	}
	void Update(SDL_Rect& camera) {
		auto entities = GetSystemEntities();
		for (int i = 0; i < entities.size(); i++) {
			auto& transform = entities[i].GetComponent<TransformComponent>();
			camera.x = transform.position.x - camera.w / 2;
			camera.y = transform.position.y - camera.h / 2;
			camera.x = camera.x < 0 ? 0 : camera.x;
			camera.y = camera.y < 0 ? 0 : camera.y;
		}
	}
};