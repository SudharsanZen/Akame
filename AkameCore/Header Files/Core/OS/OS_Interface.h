#pragma once

#include"Core/AkameCore.h"
#if defined(EXPOSE_OS_INTERFACE) || defined(AK_EXPORT)
	AKAME_API std::string SaveFileDialog(const char* filter, const char* defaultExt);
	AKAME_API std::string OpenFileDialog(const char* filter, const char* initdir);
#endif
