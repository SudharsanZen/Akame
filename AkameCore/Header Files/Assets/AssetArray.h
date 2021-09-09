#pragma once
#include<memory>
#include<vector>
#include<cassert>
#include<iostream>
template<class T>
class AssetArray
{

	std::vector<long long> freelist;
	std::vector<std::shared_ptr<T>> assetList;
	std::vector<int> refcount;


public:
	
	template<typename ...Args>
	/*Creates and stores the Assset class (texture,shader and etc), then returns the stored index*/
	long long CreateAsset(Args&& ...arguments)
	{
		if (freelist.size() > 0)
		{
			long long index = freelist.back();
			freelist.pop_back();
			assetList[index] = std::make_shared<T>(std::forward<Args>(arguments)...);
			refcount[index] = 1;
			return index;
		}
		assetList.push_back(std::make_shared<T>(std::forward<Args>(arguments)...));
		refcount.push_back(1);
		return assetList.size() - 1;
	}
	
	std::shared_ptr<T> GetAsset(unsigned long long index)
	{
		assert(assetList.size() > index && "Trying to access non existing element from the array!");
		assert(refcount[index] > 0 && "Asset was destroyed dude to zero refCount!");
		return assetList[index];
	}

	int decrementRefcount(unsigned long long index)
	{
		refcount[index]--;
		if (refcount[index] <= 0)
		{
			refcount[index] = 0;
			freelist.push_back(index);
			assetList[index].reset();
		}
		return refcount[index];
	}

	int incrementRefcount(long long index)
	{
		refcount[index]++;
		return refcount[index];
	}


};

