#include"Core/AkameCore.h"

AKAME_API void  _AKWASSERT(char const* _Message, char const* _File, unsigned _Line)
{
	
	std::cout <<_Message<<" "<<"file: "<<_File<<"Line: "<<_Line;
	throw(_Message);
}
