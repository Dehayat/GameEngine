#include "ECS.h"
#include <string>
#include "../Logger/Logger.h"

int BaseComponent::nextId = 0;

int Entity::GetId() const
{
	return id;
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
	int entityId = numEntities++;
	Entity entity(entityId);
	entity.registry = this;
	entitiesToBeAdded.insert(entity);
	if (entityId >= entityComponenentSignatures.size()) {
		entityComponenentSignatures.resize(entityId + 1);
	}
	Logger::Log("Entity created with id: " + std::to_string(entityId));
	return entity;
}

void Registry::Update()
{
	for (auto entity : entitiesToBeAdded) {
		AddEntityToSystems(entity);
	}
	entitiesToBeAdded.clear();
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

