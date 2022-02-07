#pragma once
#ifdef _WIN32
	#ifdef AK_DLL
		#ifdef AK_EXPORT
			#define AKAME_API _declspec(dllexport)
			#define AKAME_EXTERN
		#else
			#define AKAME_API _declspec(dllimport)
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