#pragma once
#include"Core/AkameCore.h"
class AkameWindowBase
{
public:
	//NOTE:
	//incase you want to initialize imgui outside the framework while using dll,
	//call this function once and set the imGui context for the dll 
	//it's derived by almost all windows class in Akame
	//if you are creating your own, just call this function from an instance of AkameWindowBase
	AKAME_API void SetImGuiContext(void* imguicontext);
};

