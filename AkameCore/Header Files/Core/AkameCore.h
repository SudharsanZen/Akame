#pragma once
#include"ECSTYPES.h"
#ifdef _WIN32
	#ifdef AK_DLL
		#ifdef AK_EXPORT
			#define AKAME_API __declspec(dllexport)
			#define AKAME_EXTERN
		#else
			#define AKAME_API __declspec(dllimport)
			#define AKAME_EXTERN extern
		#endif
	#else
		#define AKAME_API 	
	#endif
#elif AK_STATIC
	#ifndef AK_EXPORT
		#define AK_EXPORT
	#endif
#endif