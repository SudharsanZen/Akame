#include"Core/Editor/SceneHierarchyWindow/SceneHierarchyWindow.h"
#include "Core\Editor\EditorUI.h"
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include<glad\glad.h>
#include<GLFW\glfw3.h>
#include<iostream>
#include"Core/Input.h"
#include<sstream>
#include"Components/EntityDescriptor.h"
#include"Rendering/System/LightSystem.h"
#include"Rendering/System/RenderingSystem.h"
#include"Rendering/System/DeferredPipeline.h"
#include"Rendering/System/PSSMFBO.h"
#include"Core/Debug/Debug.h"

void SceneHierarchyWindow::Draw()
{
    std::shared_ptr<ECS> e = scene.ecs;
    //draw
    ImGui::Begin("SceneGraph");

    if (ImGui::Button("ToggleDebugInfo", ImVec2(200.0f, 25.0f)))
        viewDebugInfo = !viewDebugInfo;

    if (ImGui::TreeNode("Scene"))
    {

        static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        static bool align_label_with_current_x_position = false;
        static bool test_drag_and_drop = false;

        if (align_label_with_current_x_position)
            ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());


        for (auto ent : scene.transformManager->entities)
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

    ImGui::End();

    //handle inputs

    if (ImGui::IsKeyDown(KEY_DELETE))
    {

        for (auto ent : selected)
        {
            scene.ecs->DestroyEntity(ent);

        }
        selected.clear();
    }



    ImGui::Begin("properties");
    ImGui::Text("Transform");

    if (selected.size() > 0)
    {
        Transform& t = scene.GetComponent<Transform>(*selected.begin());
        glm::vec3 pose = t.GetGlobalPosition();
        glm::vec3 lpose = t.GetLocalPosition();
        glm::vec3 scale = t.GetGlobalScale();
        glm::vec3 lscale = t.GetLocalScale();
        Quaternion rot = t.GetGlobalRotation();
        Quaternion lrot = t.GetLocalRotation();
        ImGui::Text("Globalposition: x:%f  y:%f  z:%f", pose.x, pose.y, pose.z);
        ImGui::Text("LocalPosition: x:%f  y:%f  z:%f", lpose.x, lpose.y, lpose.z);

        ImGui::Text("GlobalRotation: x:%f  y:%f  z:%f w:%f", rot.quaternion.x, rot.quaternion.y, rot.quaternion.z, rot.quaternion.w);
        ImGui::Text("LocalRotation: x:%f  y:%f  z:%f w:%f", lrot.quaternion.x, lrot.quaternion.y, lrot.quaternion.z, lrot.quaternion.w);


        ImGui::Text("GlobalScale: x:%f  y:%f  z:%f", scale.x, scale.y, scale.z);
        ImGui::Text("LocalScale: x:%f  y:%f  z:%f", lscale.x, lscale.y, lscale.z);

        if (ImGui::Button("goToSelection", ImVec2(200.0f, 25.0f)))
        {
            glm::vec3 camPose = pose + scene.cam.transform.forward() * 2.0f;
            scene.cam.transform.SetGlobalPosition(camPose);
        }
    }
    ImGui::End();

    for (auto ent : selected)
    {
        if (scene.ecs->IsComponentAttached<Transform>(ent))
        {
            Transform& t = scene.GetComponent<Transform>(ent);
            if (scene.ecs->IsComponentAttached<Mesh>(ent))
            {
                Mesh& m = scene.GetComponent<Mesh>(ent);
                Debug::DrawBB(m.min, m.max, t.transformMatrix(), glm::vec3(1, 1, 1));

            }
            else if (scene.ecs->IsComponentAttached<Lights>(ent))
            {
                Lights& l = scene.GetComponent<Lights>(ent);
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
            else if (scene.ecs->IsComponentAttached<Transform>(ent))
            {
                Transform& t = scene.ecs->GetComponent<Transform>(ent);
                glm::vec3 pose = t.GetGlobalPosition();
                Debug::DrawCircle(pose, t.up(), 0.5, glm::vec3(1, 0.5, 0));
                Debug::DrawCircle(pose, t.forward(), 0.5, glm::vec3(1, 0.5, 0));
                Debug::DrawCircle(pose, t.right(), 0.5, glm::vec3(1, 0.5, 0));




            }
        }
    }

}
void SceneHierarchyWindow::DrawNode(Transform const& t, EntityDescriptor& edt, ImGuiTreeNodeFlags const& base_flags)
{

    Entity selectedEntity(-1, -1);
    int numOfChild = t.child.size();
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
                Transform& t = scene.ecs->GetComponent<Transform>(child);
                EntityDescriptor& edt = scene.ecs->GetComponent<EntityDescriptor>(child);
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
