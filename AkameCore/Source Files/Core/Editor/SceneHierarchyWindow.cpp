#include"Core/Window.h"
#include"Core/Editor/SceneHierarchyWindow/SceneHierarchyWindow.h"
#include "Core\Editor\EditorUI.h"
#include<iostream>
#include"Core/Input.h"
#include<sstream>
#include"Components/EntityDescriptor.h"
#include"Rendering/System/LightSystem.h"
#include"Rendering/System/RenderingSystem.h"
#include"Rendering/System/DeferredPipeline.h"
#include"Rendering/System/PSSMFBO.h"
#include"Core/Debug/Debug.h"
#include"ECS.h"
#pragma warning(push, 0)
#pragma warning( disable : 26495)
#pragma warning( disable : 6031)
#pragma warning( disable : 26812)
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<imgui/imgui_internal.h>
#include<glad\glad.h>
#include<GLFW\glfw3.h>
#pragma warning(pop)
void SceneHierarchyWindow::Draw()
{
    std::shared_ptr<ECS> e = m_Scene.ecs;
    //draw
    if (ImGui::Begin("SceneGraph"))
    {

        if (ImGui::Button("ToggleDebugInfo", ImVec2(200.0f, 25.0f)))
            viewDebugInfo = !viewDebugInfo;

        if (ImGui::TreeNodeEx("Scene", ImGuiTreeNodeFlags_DefaultOpen))
        {

            static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
            static bool align_label_with_current_x_position = false;
            static bool test_drag_and_drop = false;

            if (align_label_with_current_x_position)
                ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());


            for (auto ent : m_Scene.transformManager->entities)
            {
                Transform& t = e->GetComponent<Transform>(ent);
                EntityDescriptor& edt = e->GetComponent<EntityDescriptor>(ent);
                if (t.parent == Entity(-1, -1))
                {
                    DrawNode(t, edt, base_flags);
                }
            }


            ImGui::TreePop();
        }
        //handle inputs

        if (ImGui::BeginPopupContextWindow())
        {
            ImGui::MenuItem("Create Entity");
            ImGui::MenuItem("Create Camera");
            if (ImGui::BeginMenu("Primitives"))
            {

                ImGui::MenuItem("Cube");
                ImGui::MenuItem("Sphere");
                ImGui::MenuItem("Plane");
                ImGui::MenuItem("Capsule");
                ImGui::MenuItem("Cylinder");

                ImGui::EndMenu();
            }

            ImGui::EndPopup();
        }



        ImGui::End();
        for (auto ent : selected)
        {
            if (m_Scene.ecs->IsComponentAttached<Transform>(ent))
            {
                Transform& t = m_Scene.GetComponent<Transform>(ent);
                if (m_Scene.ecs->IsComponentAttached<Mesh>(ent))
                {
                    Mesh& m = m_Scene.GetComponent<Mesh>(ent);
                    Debug::DrawBB(m.min, m.max, t.transformMatrix(), glm::vec3(1, 1, 1));

                }
                else if (m_Scene.ecs->IsComponentAttached<Lights>(ent))
                {
                    Lights& l = m_Scene.GetComponent<Lights>(ent);
                    glm::vec3 pose = t.GetGlobalPosition();
                    if (l.getType() == LIGHT::POINT)
                    {
                        Debug::DrawCircle(pose, t.up(), 0.5, glm::vec3(1, 0.5, 0));
                        Debug::DrawCircle(pose, t.forward(), 0.5, glm::vec3(1, 0.5, 0));
                        Debug::DrawCircle(pose, t.right(), 0.5, glm::vec3(1, 0.5, 0));
                    }
                    else if (l.getType() == LIGHT::DIRECTIONAL)
                    {
                        Debug::DrawCircle(pose, l.getDirection(), 1, glm::vec3(1, 0.8, 1), 20);
                        Debug::DrawRay(pose, l.getDirection(), 1.5, glm::vec3(0, 0, 1));
                    }
                }
                else if (m_Scene.ecs->IsComponentAttached<Transform>(ent))
                {
                    Transform& t = m_Scene.ecs->GetComponent<Transform>(ent);
                    glm::vec3 pose = t.GetGlobalPosition();
                    Debug::DrawCircle(pose, t.up(), 0.5, glm::vec3(1, 0.5, 0));
                    Debug::DrawCircle(pose, t.forward(), 0.5, glm::vec3(1, 0.5, 0));
                    Debug::DrawCircle(pose, t.right(), 0.5, glm::vec3(1, 0.5, 0));




                }
            }

        }

        if (ImGui::IsKeyDown(KEY_DELETE))
        {

            for (auto ent : selected)
            {
                m_Scene.ecs->DestroyEntity(ent);

            }
            selected.clear();
        }
    }
}
void SceneHierarchyWindow::DrawNode(Transform const& t, EntityDescriptor& edt, ImGuiTreeNodeFlags const& base_flags)
{

    Entity selectedEntity(-1, -1);
    size_t numOfChild = t.child.size();
    static int selection_mask = (1 << 2);
    int node_clicked = -1;
    ImGuiTreeNodeFlags node_flags = base_flags;
    std::stringstream str;
    str << " id: " << edt.eid.index << "-" << edt.eid.version;
    const bool is_selected = (selected.find(edt.eid) != selected.end());
    if (is_selected)
        node_flags |= ImGuiTreeNodeFlags_Selected;
    std::string entName = edt.name;
    //tree nodes
    if (viewDebugInfo)
        entName += str.str();
    if (numOfChild > 0)
    {

        bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)edt.eid.index, node_flags, (entName).c_str(), edt.eid.index);
        if (ImGui::IsItemClicked())
        {
            selectedEntity = edt.eid;
        }

        if (node_open)
        {
            for (auto child : t.child)
            {
                Transform& t = m_Scene.ecs->GetComponent<Transform>(child);
                EntityDescriptor& edt = m_Scene.ecs->GetComponent<EntityDescriptor>(child);
                DrawNode(t, edt, base_flags);
            }
            ImGui::TreePop();
        }


    }
    else
    {
        //leaves
        node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
        ImGui::TreeNodeEx((void*)(intptr_t)edt.eid.index, node_flags, (entName).c_str(), edt.eid.index);
        if (ImGui::IsItemClicked())
            selectedEntity = edt.eid;


    }
    if (selectedEntity != Entity(-1, -1))
    {
        if (ImGui::GetIO().KeyCtrl)
        {
            if (selected.find(edt.eid) == selected.end())
                selected.insert(edt.eid);
            else
            {
                selected.erase(edt.eid);
            }
        }
        else
        {
            selected.clear();
            selected.insert(edt.eid);
        }
    }
}


SceneHierarchyWindow::SceneHierarchyWindow(Scene& s) :m_Scene(s)
{
    m_menuPosex = 0.0f;
    m_menuPosey = 0.0f;
    viewDebugInfo = false;
}
