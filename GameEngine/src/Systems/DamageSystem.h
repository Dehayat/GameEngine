#pragma once
#include "../ECS/ECS.h"
#include "../Components/BoxColliderComponent.h"
#include "../EventBus/EventBus.h"
#include "../Events/CollisionEvent.h"

class DamageSystem :public System {
public:
	DamageSystem() {
		RequireComponent<BoxColliderComponent>();
	}

	void ListenToEvents(std::unique_ptr<EventBus>& eventbus) {
		eventbus->ListenToEvent<CollisionEvent>(this, &DamageSystem::OnCollisionEvent);
	}
	void OnCollisionEvent(CollisionEvent& event) {
		Logger::Log("Damage System: Entity " + std::to_string(event.a.GetId()) + " and " + "Entity " + std::to_string(event.b.GetId()) + " Collided");
		event.a.Destroy();
		event.b.Destroy();
	}

	void Update() {

	}
};