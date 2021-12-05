#pragma once
#include <algorithm>
#include <string>
#include <SDL.h>
#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"

class RenderColliderSystem : public System {
public:
	RenderColliderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}
	void Update(SDL_Renderer* renderer) {
		std::vector<Entity> entities = GetSystemEntities();

		for (auto entity : entities) {
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& collider = entity.GetComponent<BoxColliderComponent>();

			SDL_Rect destRect{
				transform.position.x + collider.offset.x,
				transform.position.y + collider.offset.y,
				collider.width,
				collider.height
			};
			if (collider.isColliding) {
				SDL_SetRenderDrawColor(renderer, 200, 0, 0, SDL_ALPHA_OPAQUE);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 200, 0, SDL_ALPHA_OPAQUE);
			}
			SDL_RenderDrawRect(renderer, &destRect);
		}
	}
};
