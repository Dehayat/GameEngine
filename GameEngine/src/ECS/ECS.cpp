#include "ECS.h"
#include <string>
#include "../Logger/Logger.h"

int BaseComponent::nextId = 0;

int Entity::GetId() const
{
	return id;
}

void Entity::Destroy()
{
	registry->DestroyEntity(*this);
}


void System::AddEntityToSystem(Entity entity) {
	entities.push_back(entity);
}
void System::RemoveEntityFromSystem(Entity entity) {
	int i;
	for (i = 0; i < entities.size(); i++) {
		if (entities[i] == entity)
			break;
	}
	if (i < entities.size()) {
		entities.erase(entities.begin() + i);
	}
}
std::vector<Entity> System::GetSystemEntities() const {
	return entities;
}
const Signature& System::GetComponentSignature() const {
	return componentSignature;
}

Entity Registry::CreateEntity()
{
	int entityId;
	if (freeIds.empty()) {
		entityId = numEntities++;
		if (entityId >= entityComponenentSignatures.size()) {
			entityComponenentSignatures.resize(entityId + 1);
		}
	}
	else {
		entityId = freeIds.front();
		freeIds.pop_front();
	}
	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdded.insert(entity);
	Logger::Log("Entity created with id: " + std::to_string(entityId));
	return entity;
}

void Registry::DestroyEntity(Entity entity) {
	entitiesToBeRemoved.insert(entity);
}

void Registry::Update()
{
	for (auto entity : entitiesToBeAdded) {
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();

	for (auto entity : entitiesToBeRemoved) {
		RemoveEntityFromSystems(entity);
		entityComponenentSignatures[entity.GetId()].reset();
		freeIds.push_back(entity.GetId());
	}
	entitiesToBeRemoved.clear();
}

void Registry::AddEntityToSystems(Entity entity)
{
	const int entityId = entity.GetId();

	const Signature& entityComponentSignature = entityComponenentSignatures[entityId];
	for (auto& system : Systems) {
		const Signature& systemSignature = system.second->GetComponentSignature();
		if ((systemSignature & entityComponentSignature) == systemSignature) {
			system.second->AddEntityToSystem(entity);
		}
	}
}

void Registry::RemoveEntityFromSystems(Entity entity) {
	for (auto& system : Systems) {
		system.second->RemoveEntityFromSystem(entity);
	}

}

