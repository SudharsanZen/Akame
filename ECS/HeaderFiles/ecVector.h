#pragma once
#include<cassert>
template<typename T>
class ecVector
{
	T* dataArray=NULL;
	e_index size=0;

public:
	ecVector(e_index arraySize)
	{

		dataArray = new T[arraySize];
		assert(dataArray && "can't allocate memory!");
		if (dataArray)
			size = arraySize;
		else
			size = 0;
	}
	void releaseResource()
	{

		std::cout << "\n~ecVector<"<<typeid(T).name()<<">()";
		if(dataArray!=NULL)
			delete[] dataArray;
		dataArray = NULL;
		size = 0;
		
	}
	ecVector(ecVector&& obj)
	{
		releaseResource();
		dataArray = obj.dataArray;
		size = obj.size;


		obj.size = 0;
		obj.dataArray = NULL;
		obj.releaseResource();
		
	}
	ecVector(ecVector& obj)
	{

		size=obj.size ;
		dataArray=obj.dataArra;
	}

	~ecVector()
	{
		
		releaseResource();
	}
	
	T& operator [](e_index index)
	{
		assert(index<size);
		return dataArray[index];
	}
};