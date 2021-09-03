#pragma once
#include<string_view>
#include"ComponentArray.h"
class ComponentManager
{
private:
	
	std::unordered_map<ComponentID, ComponentBitPosition> compIDtoBitPose;
	std::unordered_map<ComponentID, std::shared_ptr<ComponentArrayBase>> compIDtoArray;
	ComponentBitPosition bitPoseOfNextComponent;

	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		ComponentID CID = typeid(T).name();
		
		assert(compIDtoArray.find(CID) != compIDtoArray.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(compIDtoArray[CID]);
	}

public:
	~ComponentManager()
	{
		for (auto i : compIDtoArray)
		{
			i.second.reset();
		}
	}
	ComponentManager()
	{
		bitPoseOfNextComponent = 0;
	}

	template<typename T>
	void RegisterComponent()
	{
		//Register a new component in the component Manager
		ComponentID CID=typeid(T).name();
		assert(compIDtoBitPose.find(CID) == compIDtoBitPose.end() && "component already registered!");

		compIDtoBitPose.insert({CID,bitPoseOfNextComponent});
		compIDtoArray.insert({CID,std::make_shared<ComponentArray<T>>()});
		
		bitPoseOfNextComponent++;
	}

	template<typename T>
	ComponentBitPosition GetComponentBitPose()
	{
		//get the bitPosition of the component to set bits in signature of an entity or a system
		ComponentID CID = typeid(T).name();
		
		assert(compIDtoBitPose.find(CID)!=compIDtoBitPose.end() && "no such component is registered!");

		return compIDtoBitPose[CID];
	}

	template<typename T>
	T& AddComponent(Entity entity)
	{
		return GetComponentArray<T>()->InsertData(entity);
	}

	template<typename T>
	void RemoveComponent(Entity entity)
	{
		GetComponentArray<T>()->RemoveData(entity);
	}
	template<typename T>
	T& GetComponent(Entity entity)
	{
		return GetComponentArray<T>()->GetData(entity);
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& idArrayPair : compIDtoArray)
		{
			idArrayPair.second->EntityDestroyed(entity);
		}
	}



};

