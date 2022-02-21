
#include<sstream>
#include"Core/Log/Log.h"
#include<Windows.h>
#include"Core/AkameCore.h"
void _AKWASSERT(char const* _Message, char const* _File, unsigned _Line)
{
	
	std::stringstream message;
	message<< _Message << " " << "file: " << _File << "Line: " << _Line;
	std::cout<<("ASSERT_FAILED: "+message.str());
	int id=MessageBoxA(NULL,message.str().c_str(),"Assert Failed!",MB_ABORTRETRYIGNORE);
	
	switch (id)
	{
	case 4:
		throw("Assert Failed!");
		break;
	default:
		exit(-1);
	}
}
