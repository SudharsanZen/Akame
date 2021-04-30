#include<iostream>
#include<vector>
#include<cassert>
typedef long long OBJECT_ID;

template<typename T>
class Container
{

private:
	const OBJECT_ID MAX = (((OBJECT_ID)1 << 31) * 2) - 1;
	const size_t chunk_size=256;
	std::vector<T*> objectTable;
	std::vector<OBJECT_ID> freeList;

public:
	OBJECT_ID create_object()
	{
		if (freeList.empty())
		{
			if (objectTable.size() * chunk_size >= MAX)
			{
				std::cout << "container reached max capacity can't create new object!"
				return -1;
			}
			T* objectRow = new T[chunk_size];
			for (OBJECT_ID i = 0; i < chunk_size; i++)
			{
				objectRow[i].id=objectTable.size()*chunk_size+i ;
				freeList.push_back(objectRow[i].id);
			}
			objectTable.push_back(objectRow);
		}

		OBJECT_ID freeID = freeList.back();
		freeList.pop_back();
		return object_table[freeID / chunk_size][freeID % chunk_size].id;
	}

	T& get_object(const OBJECT_ID &ID)
	{
		
	}



	
	
};