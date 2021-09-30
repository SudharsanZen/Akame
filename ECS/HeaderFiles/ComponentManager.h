#pragma once
#include<string_view>
#include"ComponentArray.h"
class ComponentManager
{
public:

	std::unordered_map<ComponentID, ComponentBitPosition> compIDtoBitPose;
	std::unordered_map<ComponentID, std::shared_ptr<ComponentArrayBase>> compIDtoArray;
	//base array pointers indexed in the order of registration
	std::vector<std::shared_ptr<ComponentArrayBase>> compBaseArraysInOrder;
	ComponentBitPosition bitPoseOfNextComponent;

	/*template<typename T>
	ComponentID getComponentID()
	{
		return typeid(T).name();
	}*/
	template<typename T>
	std::shared_ptr<ComponentArray<T>> GetComponentArray()
	{
		ComponentID CID = typeid(T).name();
		
		assert(compIDtoArray.find(CID) != compIDtoArray.end() && "Component not registered before use.");

		return std::static_pointer_cast<ComponentArray<T>>(compIDtoArray[CID]);
	}


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
		compIDtoArray[CID]->componentBitPose =GetComponentBitPose<T>();
		compBaseArraysInOrder.push_back(compIDtoArray[CID]);
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

	template<typename T>
	T& GetComponent(e_index dataIndex,e_index index)
	{
		ComponentArray<T>* upCast = (ComponentArray<T>*)compBaseArraysInOrder[index].get();
		return upCast->GetData(dataIndex);
	}

	void EntityDestroyed(Entity entity)
	{
		for (auto const& idArrayPair : compIDtoArray)
		{
			idArrayPair.second->EntityDestroyed(entity);
		}
	}



};

