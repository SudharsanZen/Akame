#include"glad/glad.h"
#include"glfw/glfw3.h"
#include<imGui\backends\imgui_impl_glfw.h>

#include "..\..\..\Header Files\Core\Editor\ImGuiUtils.h"
#include<imGui\backends\imgui_impl_opengl3.h>

void dragFloat3(std::string* names, float* values, float width)
{
    ImGui::PushItemWidth(width);
    ImGui::DragFloat(names[0].c_str(), &values[0]);   ImGui::SameLine();
    ImGui::DragFloat(names[1].c_str(), &values[1]); ImGui::SameLine();
    ImGui::DragFloat(names[2].c_str(), &values[2]);
    ImGui::PopItemWidth();
}

void DrawHeadingWithSeperator(std::string name)
{
    ImGui::Text(name.c_str());
    ImGui::Separator();
}

void dragFloat4(std::string* names, float* values, float width)
{
    ImGui::PushItemWidth(width);
    ImGui::DragFloat(names[0].c_str(), &values[0]);   ImGui::SameLine();
    ImGui::DragFloat(names[1].c_str(), &values[1]);   ImGui::SameLine();
    ImGui::DragFloat(names[2].c_str(), &values[2]);   ImGui::SameLine();
    ImGui::DragFloat(names[3].c_str(), &values[3]);
    ImGui::PopItemWidth();
}

