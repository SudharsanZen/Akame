#pragma once
#include"ECSTYPES.h"
#include<vector>
#include<unordered_map>
#include<cassert>
#include"ecVector.h"

class ComponentArrayBase
{

	public:
		ComponentBitPosition componentBitPose;
		virtual ~ComponentArrayBase() = default;
		virtual void EntityDestroyed(Entity entity)=0;
};
template<class T>
struct ComponentAllocator
{

	typedef T value_type;
	ComponentAllocator() = default;
	template<class U> 
	constexpr ComponentAllocator(const ComponentAllocator <U>&) noexcept {}
	T* allocate(std::size_t n)
	{
		if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
			throw std::bad_array_new_length();
		if (auto p = static_cast<T*>(std::malloc(n * sizeof(T))))
		{
		
			return p;
		}

		throw std::bad_alloc();
	}
	template<class U,class ...Params>
	void construct(U* ptr,Params&&... args)
	{
		new ((void*)ptr) U(std::forward<Params>(args)...);
	}
	void deallocate(T* p, std::size_t n) noexcept {
		
		std::free(p);
	}


	
};

template<typename T>
class ComponentArray:public ComponentArrayBase
{
private:
	const e_index MAXELEMENT =1000;

	std::vector<std::vector<T,ComponentAllocator<T>>>	dataList;
	std::vector<e_index> freeList;
	std::vector<e_index> usedList;
	friend class ComponentManager;
public:
	T& InsertData(Entity entity);
	void RemoveData(Entity entity);
	T& GetData(Entity entity);
	T& GetData(e_index entity);
	void EntityDestroyed(Entity entity) override;
};

template<typename T>
inline T& ComponentArray<T>::InsertData(Entity entity)
{
	
	assert(!entity.signature->test(componentBitPose)	&&	"Component Already Attached to this entity!" );

	
	if (freeList.size()==0)
	{
	
		std::vector<T,ComponentAllocator<T>> row(MAXELEMENT);
		for (e_index i = 0; i < MAXELEMENT; i++)
		{
			freeList.push_back(dataList.size()*MAXELEMENT+i);
		}
		dataList.push_back(std::move(row));
	}

	e_index freeIndex = freeList.back();

	freeList.pop_back();
	(*entity.componentIndex)[componentBitPose] = freeIndex;
	entity.signature->set(componentBitPose,true);

	
	(dataList[freeIndex / MAXELEMENT][freeIndex % MAXELEMENT].eid=entity);
	(dataList[freeIndex / MAXELEMENT][freeIndex % MAXELEMENT].eid.index=entity.index);

	return ((dataList[freeIndex / MAXELEMENT][freeIndex % MAXELEMENT]));
}

template<typename T>
inline void ComponentArray<T>::RemoveData(Entity entity)
{
	assert(entity.signature->test(componentBitPose) && "trying to remove non existing component!");
	e_index index = entity.index;
	dataList[index / MAXELEMENT][index % MAXELEMENT].reset();
	entity.signature->set(componentBitPose, false);
	(*entity.componentIndex)[componentBitPose] = ECS_INVALID_INDEX;
	freeList.push_back(index);
}

template<typename T>
inline T& ComponentArray<T>::GetData(Entity entity)
{
	// TODO: insert return statement here
	assert(entity.signature->test(componentBitPose) && "no such component is registered on the given entity!");
	e_index index = (*entity.componentIndex)[componentBitPose];
	return dataList[index / MAXELEMENT][index % MAXELEMENT];
}
template<typename T>
inline T& ComponentArray<T>::GetData(e_index dataIndex)
{
	
	return dataList[dataIndex / MAXELEMENT][dataIndex % MAXELEMENT];
}
template<typename T>
inline void ComponentArray<T>::EntityDestroyed(Entity entity)
{
	RemoveData(entity);	
}
