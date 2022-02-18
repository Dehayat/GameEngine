#pragma once
#include <string>
#include "../ECS/ECS.h"
#include "../EventBus/EventBus.h"
#include "../Events/KeyPressedEvent.h"
#include "../Logger/Logger.h"

class KeyboardMovementSystem :public System {
public:
	KeyboardMovementSystem() {}

	void ListenToEvents(std::unique_ptr<EventBus>& eventBus) {
		eventBus->ListenToEvent(this, &KeyboardMovementSystem::OnKeyPressedEvent);
	}
	void Update() {}

	void OnKeyPressedEvent(KeyPressedEvent& event) {
		Logger::Log("Key " + std::to_string(event.key) + " Pressed");
	}
};