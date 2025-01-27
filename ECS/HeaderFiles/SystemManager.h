#pragma once
#include"ECSTYPES.h"
#include"System.h"
#include<unordered_map>

class SystemManager
{
private:

	std::unordered_map<ComponentID, Signature> systemSignature;	
	std::unordered_map<ComponentID, std::shared_ptr<System>> systems;
	friend class ECS;
public:
	template<typename T, typename ...Args>
	std::shared_ptr<T>	RegisterSystem(Args&&... args)
	{
		ComponentID SID = typeid(T).name();
		
		assert(systems.find(SID) == systems.end() && "attempt to register system more than once!");

		std::shared_ptr<T> system = std::make_shared<T>(std::forward<Args>(args)...);

		systems.insert({SID,system});

		return system;
	}
	template<typename T>
	std::shared_ptr<T> GetSystemPtr()
	{
		ComponentID SID = typeid(T).name();

		assert(systems.find(SID) != systems.end() && "trying to fetch a non existing system!");
		return std::static_pointer_cast<T>(systems[SID]);
	}
	template<typename T>
	void SetSystemSignature(Signature signature)
	{
		ComponentID SID = typeid(T).name();

		assert(systems.find(SID) != systems.end() && "trying to set signature before registering the system!");
		assert(systemSignature.find(SID) == systemSignature.end() && "system's Signature already set!");
		
		systemSignature.insert({SID,signature});
		
	}

	void EntitySignatureChanged(Entity entity)
	{
		Signature entitySig = *(entity.signature);
		for (auto const& pair : systems)
		{
			ComponentID const &SID = pair.first;
			auto const &system= pair.second;
			Signature const &sysSig = systemSignature[SID];
			
			if ((sysSig & entitySig)==sysSig)
			{
				system->entities.insert(entity);
				system->OnAddEntity(entity);
			}
			else
			{
				

				system->entities.erase(entity);
				system->AfterDestroyEntity();
			}
		}
	}
	
	void EntityDestroyed(Entity entity)
	{
		for (auto const& pair : systems)
		{
		
			pair.second->entities.erase(entity);
			pair.second->AfterDestroyEntity();
		}
	}

};