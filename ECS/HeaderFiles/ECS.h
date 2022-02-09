#pragma once
#include"ComponentManager.h"
#include"SystemManager.h"

class ECS
{
private:
	
	e_index maxEntities = 1000;
	std::list<e_index> freeList;
	std::vector<std::vector<Entity>> entityList;


	

	//checks if the entity is valid and returns the value from the list
	Entity& GetEntityFromList(Entity entity)
	{
		e_index a = entityList.size();
		assert((entity.index < (a* maxEntities)) && "entity index out of range");

		Entity& listValue = entityList[entity.index / maxEntities][entity.index % maxEntities];

		assert(entity == listValue && listValue.entityAlive && "trying to destroy a non existing entity!");
		return listValue;
	}
public:
	ComponentManager componentManager;
	SystemManager systemManager;
	ECS()
	{
		
	}

	Entity CreateEntity()
	{
		if (freeList.size()==0)
		{
			std::vector<Entity> newRow = std::vector<Entity>(maxEntities);
			e_index startIndex = entityList.size()*maxEntities;
			for (e_index i = 0; i < maxEntities; i++)
			{
				newRow[i]=Entity(startIndex+i,0);
				newRow[i].signature = std::make_shared<Signature>();
				newRow[i].componentIndex = std::make_shared<std::vector<e_index>>(newRow[i].signature->size());
				freeList.push_back(newRow[i].index);
			}
			entityList.push_back(newRow);
		}
		e_index index = freeList.front();
		freeList.pop_front();
		Entity available = entityList[index/maxEntities][index%maxEntities];
		return available;
	}

	void DestroyEntity(Entity entity)
	{
		

		Entity &listValue =GetEntityFromList(entity);


		for (auto pair : systemManager.systems)
		{
			auto& system = pair.second;
			if (system->entities.find(entity) != system->entities.end())
				system->OnDestroyEntity(entity);
		}
		componentManager.EntityDestroyed(entity);
		systemManager.EntityDestroyed(entity);
		
		listValue.version++;
		listValue.signature->reset();
		for (auto& index : *listValue.componentIndex)
		{
			index = ECS_INVALID_INDEX;
		}
		freeList.push_back(entity.index);
		
		
	}

	template<typename T>
	void RegisterComponent()
	{
		componentManager.RegisterComponent<T>();
	}

	template<typename T>
	T& AddComponent(Entity entity)
	{
		
		Entity& listValue = GetEntityFromList(entity);
		T& compRef=componentManager.AddComponent<T>(listValue);
		

		//listValue.signature->set(componentManager.GetComponentBitPose<T>(),true);
		systemManager.EntitySignatureChanged(entity);

		return compRef;
	}

	
	template<typename T>
	T& GetComponent(Entity entity)
	{
		Entity& listValue = GetEntityFromList(entity);
		return componentManager.GetComponent<T>(listValue);
	}
	template<typename T>
	T& GetComponent(e_index dataIndex, e_index index)
	{
		return componentManager.GetComponent<T>(dataIndex,index);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		
		componentManager.RemoveComponent<T>(entity);

		entity.signature->set(componentManager.GetComponentBitPose<T>(), false);
		systemManager.EntitySignatureChanged(entity);
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

	Signature GetEntitySignature(Entity entity)
	{
		Entity& listValue = GetEntityFromList(entity);
		
		return *listValue.signature;
	}
	template<typename T>
	bool IsComponentAttached(Entity entity)
	{
		return (entity.signature->test(GetComponentBitPose<T>()));
	}

	bool IsEntityValid(Entity entity)
	{
		if ((entity.index > entityList.size() * maxEntities))
			return false;

		Entity& listValue = entityList[entity.index / maxEntities][entity.index % maxEntities];

		return (entity == listValue && listValue.entityAlive);
	}
	~ECS()
	{
		std::cout << "\n~ECS()\n";
	}
};