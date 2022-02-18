#pragma once
#include <string>
#include <SDL_keycode.h>
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Logger/Logger.h"
#include "../Components/SpriteComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Components/KeyboardMovementComponent.h"

class KeyboardMovementSystem :public System {
public:
	KeyboardMovementSystem() {
		RequireComponent<SpriteComponent>();
		RequireComponent<RigidBodyComponent>();
		RequireComponent<KeyboardMovementComponent>();
	}

	void ListenToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->ListenToEvent(this, &KeyboardMovementSystem::OnKeyPressedEvent);
	}
	void Update() {}

	void OnKeyPressedEvent(KeyPressedEvent& event) {
		auto entities = GetSystemEntities();
		for (int i = 0; i < entities.size(); i++) {
			auto& moveComponent = entities[i].GetComponent<KeyboardMovementComponent>();
			auto& rigidBodyComponent = entities[i].GetComponent<RigidBodyComponent>();
			auto& spriteComponent = entities[i].GetComponent<SpriteComponent>();
			if (event.key == SDLK_UP) {
				spriteComponent.srcRect.y = spriteComponent.height * 0;
				rigidBodyComponent.velocity = glm::vec2(0.0f, -1.0f) * moveComponent.moveSpeed;
			}
			else if (event.key == SDLK_RIGHT) {
				spriteComponent.srcRect.y = spriteComponent.height * 1;
				rigidBodyComponent.velocity = glm::vec2(1.0f, 0.0f) * moveComponent.moveSpeed;
			}
			else if (event.key == SDLK_DOWN) {
				spriteComponent.srcRect.y = spriteComponent.height * 2;
				rigidBodyComponent.velocity = glm::vec2(0.0f, 1.0f) * moveComponent.moveSpeed;
			}
			else if (event.key == SDLK_LEFT) {
				spriteComponent.srcRect.y = spriteComponent.height * 3;
				rigidBodyComponent.velocity = glm::vec2(-1.0f, 0.0f) * moveComponent.moveSpeed;
			}
		}
	}
};