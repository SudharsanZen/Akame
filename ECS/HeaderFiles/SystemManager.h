#pragma once
#include"ECSTYPES.h"
#include"System.h"
#include<unordered_map>

class SystemManager
{
private:

	std::unordered_map<ComponentID, Signature> systemSignature;	
	std::unordered_map<ComponentID, std::shared_ptr<System>> systems;

public:
	template<typename T>
	std::shared_ptr<T>	RegisterSystem()
	{
		ComponentID SID = typeid(T).name();
		
		assert(systems.find(SID) == systems.end() && "attempt to register system more than once!");

		std::shared_ptr<T> system = std::make_shared<T>();

		systems.insert({SID,system});

		return system;
	}

	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		ComponentID SID = typeid(T).name();

		assert(systems.find(SID) != systems.end() && "trying to set signature before registering the system!");
		assert(systemSignature.find(SID) == systemSignature.end() && "system's Signature already set!");
		
		systemSignature.insert({SID,signature});
		
	}

	void EntitySignatureChanged(Entity entity,Signature signature)
	{
		for (auto const& pair : systems)
		{
			ComponentID const &SID = pair.first;
			auto const &system= pair.second;
			Signature const &sysSig = systemSignature[SID];
			
			if ((sysSig & signature)==sysSig)
			{
				system->entities.insert(entity);
				system->OnAddEntity();
			}
			else
			{
				system->entities.erase(entity);
			
			}
		}
	}
	
	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : systems)
		{
			pair.second->entities.erase(entity);
		}
	}

};