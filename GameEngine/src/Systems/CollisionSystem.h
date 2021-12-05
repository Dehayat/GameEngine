#pragma once
#include <SDL.h>
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/BoxColliderComponent.h"
#include "../Logger/Logger.h"
#include <string>

class CollisionSystem : public System {
private:
	bool CheckAABBCollision(SDL_Rect aCollider, SDL_Rect bCollider) {
		if (aCollider.x > bCollider.x + bCollider.w)return false;
		if (aCollider.y > bCollider.y + bCollider.h)return false;
		if (aCollider.x + aCollider.w < bCollider.x)return false;
		if (aCollider.y + aCollider.h < bCollider.y)return false;
		return true;
	}

public:
	CollisionSystem() {
		RequireComponent<TransformComponent>();
		RequireComponent<BoxColliderComponent>();
	}
	void Update() {
		auto entities = GetSystemEntities();
		for (int i = 0; i < entities.size(); i++) {
			auto& collider = entities[i].GetComponent<BoxColliderComponent>();
			collider.isColliding = false;
		}
		for (int i = 0; i < entities.size(); i++) {
			for (int j = i + 1; j < entities.size(); j++) {
				auto& ABoxCollider = entities[i].GetComponent<BoxColliderComponent>();
				const auto& ATransform = entities[i].GetComponent<TransformComponent>();
				auto& BBoxCollider = entities[j].GetComponent<BoxColliderComponent>();
				const auto& BTransform = entities[j].GetComponent<TransformComponent>();
				SDL_Rect aCollider{
					ATransform.position.x + ABoxCollider.offset.x,
					ATransform.position.y + ABoxCollider.offset.y,
					ABoxCollider.width,
					ABoxCollider.height
				};
				SDL_Rect bCollider{
					BTransform.position.x + BBoxCollider.offset.x,
					BTransform.position.y + BBoxCollider.offset.y,
					BBoxCollider.width,
					BBoxCollider.height
				};
				if (CheckAABBCollision(aCollider, bCollider)) {
					ABoxCollider.isColliding = true;
					BBoxCollider.isColliding = true;
					Logger::Log("Entity " + std::to_string(entities[i].GetId()) + " and " + "Entity " + std::to_string(entities[j].GetId()) + " Are colliding");
				}
			}
		}
	}
};
