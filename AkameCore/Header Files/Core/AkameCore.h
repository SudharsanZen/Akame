#pragma once
#include"System.h"
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



#if defined(AK_DEBUG) || defined(AK_EXPORT)
	AKAME_API void _AKWASSERT(char const* _Message, char const* _File, unsigned    _Line);
#if defined(AK_DEBUG)
	#define AK_ASSERT(expression) (void)(                                                       \
            (!!(expression)) ||																	\
            (_AKWASSERT((#expression), (__FILE__), (unsigned)(__LINE__)), 0)					\
        )
	#endif
#else
	#define AK_ASSERT(expressiong)
#endif