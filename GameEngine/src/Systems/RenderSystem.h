#pragma once
#include <algorithm>
#include <string>
#include <SDL.h>
#include "../ECS/ECS.h"
#include "../Components/SpriteComponent.h"
#include "../Components/TransformComponent.h"
#include "../Logger/Logger.h"
#include "../AssetStore/AssetStore.h"

class RenderSystem : public System {
public:
	RenderSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<SpriteComponent>();
	}
	void Update(SDL_Renderer* renderer, std::unique_ptr<AssetStore>& assetStore, SDL_Rect& camera) {
		std::vector<Entity> entities = GetSystemEntities();
		std::sort(entities.begin(), entities.end(),
			[](Entity& a, Entity& b) ->bool {
				SpriteComponent spriteA = a.GetComponent<SpriteComponent>();
				SpriteComponent spriteB = b.GetComponent<SpriteComponent>();
				return spriteA.zIndex < spriteB.zIndex;
			}
		);

		for (auto entity : entities) {
			const auto& transform = entity.GetComponent<TransformComponent>();
			const auto& sprite = entity.GetComponent<SpriteComponent>();


			SDL_Rect destRect{
				transform.position.x - camera.x,
				transform.position.y - camera.y,
				sprite.width * transform.scale.x,
				sprite.height * transform.scale.y
			};
			if (sprite.isFixed) {
				destRect.x = transform.position.x;
				destRect.y = transform.position.y;
			}
			SDL_RenderCopyEx(
				renderer,
				assetStore->GetTexture(sprite.assetId),
				&(sprite.srcRect),
				&destRect,
				transform.rotation,
				NULL,
				SDL_FLIP_NONE
			);
		}

	}
};
