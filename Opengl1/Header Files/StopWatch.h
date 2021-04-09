#pragma once
#include<chrono>
#include<iostream>
class StopWatch
{
private:

	std::chrono::steady_clock::time_point st;
	std::chrono::steady_clock::time_point e;
	std::chrono::duration<double> timeElapsed;

public:
	
	void start()
	{
		st = std::chrono::high_resolution_clock::now();
	}

	void end()
	{
		e = std::chrono::high_resolution_clock::now();
	}


	void printTime()
	{
		timeElapsed = (e - st);
		timeElapsed = (timeElapsed * 1000000000.0);
		std::cout << timeElapsed.count()<<std::endl;
	}
};

