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
	#endif

	#ifdef AK_STATIC 
		#define AKAME_API
		#define AKAME_EXTERN
	#endif
#endif



