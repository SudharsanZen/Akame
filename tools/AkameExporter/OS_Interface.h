#pragma once
#include<iostream>

#ifdef EXPOSE_OS_INTERFACE
	std::string SaveFileDialog(const char* filter, const char* defaultExt);
	std::string OpenFileDialog(const char* filter, const char* initdir);
#endif
