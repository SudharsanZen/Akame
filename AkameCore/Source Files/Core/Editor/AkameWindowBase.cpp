#include "Core\Editor\AkameWindowBase.h"
#pragma warning(push, 0)
#pragma warning(disable : 26495)
#pragma warning(disable : 6031)
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#pragma warning(pop)
void AkameWindowBase::SetImGuiContext(void* imguicontext)
{
	ImGui::SetCurrentContext((ImGuiContext*)imguicontext);
}
