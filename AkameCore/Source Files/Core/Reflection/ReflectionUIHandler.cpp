#include "..\..\..\Header Files\Core\Reflection\ReflectionUIHandler.h"
#include"glad/glad.h"
#include"glfw/glfw3.h"
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<Core/Editor/ImGuiUtils.h>
#include<vector>

void ReflectionUIHandler::DrawTitle(std::string name)
{
	DrawHeadingWithSeperator(name);
}
void ReflectionUIHandler::DrawUIField(std::string name, int *val)
{
	ImGui::DragInt(("##"+name).c_str(), val);
}

void ReflectionUIHandler::DrawUIField(std::string name, float *val)
{
	ImGui::DragFloat(("##" + name).c_str(),val);
}

void ReflectionUIHandler::DrawUIField(std::string name, double* val)
{
	ImGui::InputDouble(("##" + name).c_str(),val);
}

void ReflectionUIHandler::DrawUIField(std::string name, glm::vec3* val)
{

	std::string names[] = {"x","y","z"};
	
	dragFloat3(names,(float*)val,80);
}

void ReflectionUIHandler::DrawUIField(std::string name, glm::vec4* val)
{
	
	std::string names[] = { "x","y","z","w"};

	dragFloat4(names, (float*)val, 80);
}

void ReflectionUIHandler::DrawUIField(std::string name, bool* val)
{

	ImGui::Checkbox(("##" + name).c_str(),val);
}

void ReflectionUIHandler::DrawUIField(std::string name, Quaternion* val)
{
	DrawUIField(name,&val->quaternion);
}

void ReflectionUIHandler::DrawUIField(std::string name, glm::quat* val)
{
	static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
	if (ImGui::BeginTable("rotation", 4, flags))
	{
		ImGui::TableNextRow();
		ImGui::TableSetColumnIndex(0);
		ImGui::TextDisabled("w: %f", (*val).w);
		ImGui::TableSetColumnIndex(1);
		ImGui::TextDisabled("x: %f", (*val).x);
		ImGui::TableSetColumnIndex(2);
		ImGui::TextDisabled("y: %f", (*val).y);
		ImGui::TableSetColumnIndex(3);
		ImGui::TextDisabled("z: %f", (*val).z);
		ImGui::EndTable();
	}
}

void ReflectionUIHandler::DrawUIField(std::string name, std::string* val)
{
	char valc[100];
	strcpy(valc,val->c_str());

	ImGui::InputText(("##"+name).c_str(),valc,100);
	*val = valc;
}

