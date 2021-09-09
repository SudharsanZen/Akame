#pragma once
#include"ComponentManager.h"
#include"SystemManager.h"

class ECS
{
private:
	
	Entity MAXENTITIES;
	
	std::vector<Entity> versionOfEntity;
	std::vector<Signature> signature;
	std::vector<Entity> availableEntity;

	ComponentManager componentManager;
	SystemManager systemManager;

	
public:
	ECS(Entity maxEntities=20000)
	{
		MAXENTITIES = maxEntities;
		
		versionOfEntity.resize(maxEntities,0);
		signature.resize(maxEntities,Signature(0));
		
		for (uint32_t i = 0; i < MAXENTITIES; i++)
			availableEntity.push_back(i);
		
	}

	EntityID CreateEntity()
	{
		assert(availableEntity.size() != 0 && "Entity count reached MAXENTITIES, set maxEntities to a higher number while initializing ECS");
		
		Entity availableIndex= availableEntity.back();
		availableEntity.pop_back();

		Entity version = versionOfEntity[availableIndex];
		EntityID entityID;

		entityID.index = availableIndex;
		entityID.version = version;

		return entityID;
	}

	void DestroyEntity(EntityID entityID)
	{
		assert(versionOfEntity[entityID.index] == entityID.version && "trying to destroy a non existing entity!");

		for (auto pair : systemManager.systems)
		{
			auto& system = pair.second;
			if (system->entities.find(entityID.index) != system->entities.end())
				system->OnDestroyEntity(entityID.index);
		}
		componentManager.EntityDestroyed(entityID.index);
		systemManager.EntityDestroyed(entityID.index);
		
		versionOfEntity[entityID.index]++;
		availableEntity.push_back(entityID.index);
		
		signature[entityID.index].reset();
	}

	template<typename T>
	void RegisterComponent()
	{
		componentManager.RegisterComponent<T>();
	}

	template<typename T>
	T& AddComponent(EntityID entityID)
	{
		assert(versionOfEntity[entityID.index] == entityID.version && "trying to add component to a non existing entity!");
		T& compRef=componentManager.AddComponent<T>(entityID.index);
		Signature &sigComp = signature[entityID.index];

		sigComp.set(componentManager.GetComponentBitPose<T>(),true);
		systemManager.EntitySignatureChanged(entityID.index,signature[entityID.index]);

		return compRef;
	}

	template<typename T>
	T& GetComponent(EntityID entityID)
	{
		assert(versionOfEntity[entityID.index] == entityID.version && "trying to get component from a non existing entity!");
		return componentManager.GetComponent<T>(entityID.index);
	}
	
	template<typename T>
	T& GetComponent(Entity entityID)
	{
		return componentManager.GetComponent<T>(entityID);
	}
	

	template<typename T>
	void RemoveComponent(EntityID entityID)
	{
		assert(versionOfEntity[entityID.index] == entityID.version && "trying to remove component from a non existing entity!");
		componentManager.RemoveComponent<T>(entityID.index);
		Signature& sigComp = signature[entityID.index];

		sigComp.set(componentManager.GetComponentBitPose<T>(), false);
		systemManager.EntitySignatureChanged(entityID.index, signature[entityID.index]);
	}

	template<typename T>
	std::shared_ptr<T> RegisterSystem()
	{
		return systemManager.RegisterSystem<T>();
	}

	template<typename T>
	void SetSystemSignature(Signature sysSig)
	{
		systemManager.SetSystemSignature<T>(sysSig);
	}

	template<typename T>
	ComponentBitPosition GetComponentBitPose()
	{
		return componentManager.GetComponentBitPose<T>();
	}

	~ECS()
	{
		std::cout << "\n~ECS()\n";
	}
};