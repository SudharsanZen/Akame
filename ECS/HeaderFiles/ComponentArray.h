#pragma once
#include"ECSTYPES.h"
#include<vector>
#include<unordered_map>
#include<cassert>


class ComponentArrayBase
{
	public:
		virtual ~ComponentArrayBase() = default;
		virtual void EntityDestroyed(Entity entity)=0;
};

template<typename T>
class ComponentArray:public ComponentArrayBase
{
private:
	const Entity MAXELEMENT =1000;
	//ihave a vector of a vector here
	std::vector<std::vector<T>>	dataList;
	std::vector<Entity>	freeList;
	std::unordered_map<Entity, Entity> entityToIndex;

public:
	void InsertData(Entity entity, T data);
	void RemoveData(Entity entity);
	T& GetData(Entity entity);
	void EntityDestroyed(Entity entity) override;
};

template<typename T>
inline void ComponentArray<T>::InsertData(Entity entity, T data)
{
	
	assert(entityToIndex.find(entity)==entityToIndex.end()	&&	"Component Already Attached to this entity!" );
	
	
	if (freeList.size()==0)
	{
	
		std::vector<T> row(MAXELEMENT);
		for (size_t i = 0; i < MAXELEMENT; i++)
		{
			freeList.push_back(dataList.size()*MAXELEMENT+i);
		}
		dataList.push_back(std::move(row));
	}

	Entity freeIndex = freeList.back();
	freeList.pop_back();
	entityToIndex[entity] = freeIndex;
	dataList[freeIndex / MAXELEMENT][freeIndex % MAXELEMENT] = data;
}

template<typename T>
inline void ComponentArray<T>::RemoveData(Entity entity)
{
	assert(entityToIndex.find(entity) != entityToIndex.end() && "No such component is attached to this entity!");
	Entity index = entityToIndex[entity];
	dataList[index / MAXELEMENT][index % MAXELEMENT].reset();
	entityToIndex.erase(entity);
	freeList.push_back(index);
}

template<typename T>
inline T& ComponentArray<T>::GetData(Entity entity)
{
	// TODO: insert return statement here
	assert(entityToIndex.find(entity)!=entityToIndex.end() && "no such component is registered on the given entity!");
	Entity index = entityToIndex[entity];
	return dataList[index / MAXELEMENT][index % MAXELEMENT];
}

template<typename T>
inline void ComponentArray<T>::EntityDestroyed(Entity entity)
{
	if (entityToIndex.find(entity) != entityToIndex.end())
	{
		RemoveData(entity);
	}
}
