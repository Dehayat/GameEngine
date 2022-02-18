#pragma once
#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include "../Logger/Logger.h"
#include <deque>

class Entity {
private:
	int id;
public:
	Entity(int id) :id(id) { registry = nullptr; }
	int GetId() const;
	bool operator ==(const Entity& other) const { return id == other.id; }
	bool operator !=(const Entity& other) const { return id != other.id; }
	bool operator <(const Entity& other) const { return id < other.id; }

	template <typename T, typename ...TArgs>void AddComponent(TArgs&& ...args);
	template <typename T>bool HasComponent();
	template <typename T>T& GetComponent();
	template <typename T>void RemoveComponent();

	void Destroy();

	class Registry* registry;
};

struct BaseComponent {
protected:
	static int nextId;
};

template<typename T>
class Component : public BaseComponent {
public:
	static int GetId() {
		static int id = ++nextId;
		return id;
	}
};

const int MAX_COMPONENETS = 100;
typedef std::bitset<MAX_COMPONENETS> Signature;

class System {
private:
	Signature componentSignature;
	std::vector<Entity> entities;
public:
	System() = default;
	~System() = default;

	void AddEntityToSystem(Entity entity);
	void RemoveEntityFromSystem(Entity entity);
	std::vector<Entity> GetSystemEntities() const;
	const Signature& GetComponentSignature() const;
	template<typename T> void RequireComponent();
};

class IPool {
public:
	virtual ~IPool() = default;
};

template <typename T>
class Pool :public IPool {
private:
	std::vector<T> data;

public:
	Pool(int size = 100) {
		data.resize(size);
	}
	~Pool() override = default;

	bool IsEmpty() const {
		return data.empty();
	}
	bool GetSize() const {
		return data.size();
	}
	void Resize(int size) {
		data.resize(size);
	}
	void Clear() {
		data.clear();
	}
	void Add(T object) {
		data.push_back(object);
	}
	void Set(int index, T object) {
		data[index] = object;
	}
	T& Get(int index) {
		return data[index];
	}
	T& operator[](unsigned int index) {
		return data[index];
	}
};

class Registry {
private:
	int numEntities = 0;
	std::vector<std::shared_ptr<IPool>> componentPools;
	std::vector<Signature> entityComponenentSignatures;
	std::unordered_map<std::type_index, std::shared_ptr<System>> Systems;
	std::set<Entity> entitiesToBeAdded;
	std::set<Entity> entitiesToBeRemoved;
	std::deque<int> freeIds;

public:
	Registry() = default;
	Entity CreateEntity();
	void DestroyEntity(Entity entity);
	void Update();
	template<typename T, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);
	template<typename T> void RemoveComponent(Entity entity);
	template<typename T> bool HasComponent(Entity entity);
	template<typename T> T& GetComponent(Entity entity) const;

	template<typename T, typename ...TArgs> void AddSystem(TArgs&& ...args);
	template<typename T> void RemoveSystem();
	template<typename T> bool HasSystem() const;
	template<typename T> T& GetSystem() const;

	void AddEntityToSystems(Entity entity);
	void RemoveEntityFromSystems(Entity entity);
};

template<typename T>
void System::RequireComponent() {
	const int componentId = Component<T>::GetId();
	componentSignature.set(componentId);
}


template<typename T, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs && ...args)
{
	const int componentId = Component<T>::GetId();
	const int entityId = entity.GetId();
	if (componentId >= (int)componentPools.size()) {
		componentPools.resize(componentId + 1, nullptr);
	}
	if (componentPools[componentId] == nullptr) {
		std::shared_ptr<Pool<T>> newComponentPool = std::make_shared<Pool<T>>();
		componentPools[componentId] = newComponentPool;
	}

	std::shared_ptr<Pool<T>> componentPool = std::static_pointer_cast<Pool<T>>(componentPools[componentId]);

	if (entityId >= componentPool->GetSize()) {
		componentPool->Resize(entityId + 1);
	}

	T newComponent(std::forward<TArgs>(args)...);

	componentPool->Set(entityId, newComponent);

	entityComponenentSignatures[entityId].set(componentId);
}

template<typename T>
void Registry::RemoveComponent(Entity entity)
{
	const int entityId = entity.GetId();
	const int componentId = Component<T>::GetId();

	entityComponenentSignatures[entityId].set(componentId, false);
}

template<typename T>
bool Registry::HasComponent(Entity entity)
{
	const int entityId = entity.GetId();
	const int componentId = Component<T>::GetId();
	return entityComponenentSignatures[entityId].test(componentId);
}

template<typename T>
T& Registry::GetComponent(Entity entity) const
{
	const int entityId = entity.GetId();
	const int componentId = Component<T>::GetId();
	auto componentPool = std::static_pointer_cast<Pool<T>>(componentPools[componentId]);
	return componentPool->Get(entityId);
}

template<typename T, typename ...TArgs>
void Registry::AddSystem(TArgs && ...args)
{
	std::shared_ptr<T> newSystem = std::make_shared<T>(std::forward<TArgs>(args)...);
	Systems.insert(std::make_pair(std::type_index(typeid(T)), newSystem));
}

template<typename T>
void Registry::RemoveSystem()
{
	auto system = Systems.find(std::type_index(typeid(T)));
	Systems.erase(system);
}

template<typename T>
bool Registry::HasSystem() const
{
	auto system = Systems.find(std::type_index(typeid(T)));
	return system != Systems.end();
}

template<typename T>
T& Registry::GetSystem() const
{
	auto system = Systems.find(std::type_index(typeid(T)));
	return *(std::static_pointer_cast<T>(system->second));
}

template<typename T, typename ...TArgs>
void Entity::AddComponent(TArgs && ...args)
{
	registry->AddComponent<T>(*this, std::forward<TArgs>(args)...);
}

template<typename T>
bool Entity::HasComponent()
{
	return registry->HasComponent<T>(*this);
}

template<typename T>
T& Entity::GetComponent()
{
	return registry->GetComponent<T>(*this);
}

template<typename T>
void Entity::RemoveComponent()
{
	registry->RemoveComponent<T>(*this);
}
