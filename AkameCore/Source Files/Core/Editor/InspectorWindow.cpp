#include"Core/Window.h"
#include "Core/Editor/PropertiesWindow/InspectorWindow.h"
#include"Core/Editor/SceneHierarchyWindow/SceneHierarchyWindow.h"
#include "Core\Editor\EditorUI.h"
#include<Core/Editor/ImGuiUtils.h>
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
#include"Components/Animation/SkeletalMesh.h"
#include"Components/Animation/AnimationController.h"
#include"Core/Debug/Debug.h"

glm::vec3 Mat3ToEuler(glm::mat3 rot)
{
    //look http://eecs.qmul.ac.uk/~gslabaugh/publications/euler.pdf for how this works

    constexpr float pi = glm::pi<float>();

    float theta1 = -glm::asin(rot[2][0]);
    float theta2 = pi - theta1;
    if (glm::abs(rot[2][0])!=1)
    {
        float psi = glm::atan(rot[2][1] / glm::cos(theta1), rot[2][2] / glm::cos(theta1));
        float phi = glm::atan(rot[1][0] / glm::cos(theta1), rot[0][0] / glm::cos(theta1));
        return glm::degrees(glm::vec3(psi, theta1, phi));
    }
    else
    {
        //psi-phi
        float phi = 0.0f;//phi can be anything
        float sign = (rot[2][0]==-1.0f)?1.0f:-1.0f;
        float theta = (pi / 2.0f)*(sign);
        float psi_phi =phi*sign+ glm::atan(sign*rot[0][1], sign*rot[0][2]);
        return glm::degrees(glm::vec3(psi_phi, theta, phi));
    }
   
}
void InspectorWindow::DrawTransformComponent(Entity selected)
{
    static bool local=false;
    //if the entity doesn't own a Transform component then do nothing
    if (!selected.signature->test(m_Transform_pose))
        return;
    
    if (ImGui::CollapsingHeader("Transform Component",ImGuiTreeNodeFlags_DefaultOpen))
    {
        Transform& t = m_Scene.GetComponent<Transform>(selected);
        glm::vec3 pose = t.GetGlobalPosition();
        glm::vec3 lpose = t.GetLocalPosition();
        glm::vec3 scale = t.GetGlobalScale();
        glm::vec3 lscale = t.GetLocalScale();
        //Quaternion rot = t.GetGlobalRotation();
        Quaternion lrot = t.GetLocalRotation();
     
        float lRotQuat[4] = { lrot.quaternion.w,lrot.quaternion.x,lrot.quaternion.y,lrot.quaternion.z};
        if (ImGui::Button("go_To_Selection"))
        {
            glm::vec3 camPose = pose - m_Scene.cam.transform.forward() * 5.0f;
            m_Scene.cam.transform.SetGlobalPosition(camPose);
        }
        //t.SetLocalPosition(glm::vec3(lPose[0], lPose[1], lPose[2]));
        if (ImGui::BeginTabBar("##tabs", ImGuiTabBarFlags_None))
        {
            ImGui::Checkbox("show Local", &local);
            if (!local)
            {
                ImGui::Text("position");
                ImGui::Separator();
                ImGui::PushItemWidth(80);

                ImGui::DragFloat("x##p", &pose.x);   ImGui::SameLine();
                ImGui::DragFloat("y##p", &pose.y); ImGui::SameLine();
                ImGui::DragFloat("z##p", &pose.z); 
                
                t.SetGlobalPosition(pose);

                ImGui::Text("Scale");
                ImGui::Separator();
                ImGui::DragFloat("x##s", &scale.x); ImGui::SameLine();
                ImGui::DragFloat("y##s", &scale.y); ImGui::SameLine();
                ImGui::DragFloat("z##s", &scale.z);
                ImGui::PopItemWidth();
                t.SetGlobalScale(scale);
                //ImGui::EndTabItem();
            }
            else
            {
                ImGui::PushItemWidth(80);
                ImGui::Text("position");
                ImGui::Separator();
                ImGui::DragFloat("x##lp", &lpose.x);   ImGui::SameLine();
                ImGui::DragFloat("y##lp", &lpose.y); ImGui::SameLine();
                ImGui::DragFloat("z##lp", &lpose.z);
                t.SetLocalPosition(lpose);


                ImGui::Text("scale");
                ImGui::Separator();
                ImGui::DragFloat("x##ls", &lscale.x);   ImGui::SameLine();
                ImGui::DragFloat("y##ls", &lscale.y); ImGui::SameLine();
                ImGui::DragFloat("z##ls", &lscale.z);
                ImGui::PopItemWidth();
                t.SetLocalScale(lscale);
                static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
          
                ImGui::Text("rotation:");
                ImGui::Separator();
                ImGui::TextDisabled("Local rotation Quaternion:");
                if(ImGui::BeginTable("rotation", 4,flags))
                {
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::TextDisabled("w: %f",lRotQuat[0]);
                    ImGui::TableSetColumnIndex(1);
                    ImGui::TextDisabled("x: %f", lRotQuat[1]);
                    ImGui::TableSetColumnIndex(2);
                    ImGui::TextDisabled("y: %f", lRotQuat[2]);
                    ImGui::TableSetColumnIndex(3);
                    ImGui::TextDisabled("z: %f", lRotQuat[3]);
                    ImGui::EndTable();
                }
                
                ImGui::Text("Add Euler rotation to existing rotation");
                glm::quat lrotq= lrot.quaternion;
                glm::mat3 rotMat = glm::toMat3(lrotq);
                glm::vec3 eul = Mat3ToEuler(rotMat);
                float lRotEu[3] = {eul.x,eul.y,eul.z};   
          
                if (ImGui::InputFloat3("", lRotEu) )
                {
                   
                    if (ImGui::IsKeyDown(KEY_ENTER))
                    {
                        Quaternion diffRot = Quaternion((lRotEu[0]), (lRotEu[1]), (lRotEu[2]));
                        t.SetLocalRotation(diffRot);
                    }
                    
                }
                if (ImGui::Button("ResetRotation"))
                {
                    t.SetLocalRotation(Quaternion());
                }

              
                //ImGui::EndTabItem();
            }
            
        }
        /*
        ImGui::Text("Globalposition: x:%f  y:%f  z:%f", pose.x, pose.y, pose.z);
        ImGui::Text("LocalPosition: x:%f  y:%f  z:%f", lpose.x, lpose.y, lpose.z);

        ImGui::Text("GlobalRotation: x:%f  y:%f  z:%f w:%f", rot.quaternion.x, rot.quaternion.y, rot.quaternion.z, rot.quaternion.w);
        ImGui::Text("LocalRotation: x:%f  y:%f  z:%f w:%f", lrot.quaternion.x, lrot.quaternion.y, lrot.quaternion.z, lrot.quaternion.w);


        ImGui::Text("GlobalScale: x:%f  y:%f  z:%f", scale.x, scale.y, scale.z);
        ImGui::Text("LocalScale: x:%f  y:%f  z:%f", lscale.x, lscale.y, lscale.z);
        */
       
    }

}
void InspectorWindow::DrawLightComponent(Entity selected) 
{
    

    if (ImGui::CollapsingHeader("LightComponent", ImGuiTreeNodeFlags_DefaultOpen))
    {
        Lights& t = m_Scene.GetComponent<Lights>(selected);
        LIGHT type = t.getType();
        ImGui::Text("Type");
        ImGui::Separator();
        const char* typeNames[] = {"Directional","Point","Spot"};
        if (ImGui::BeginCombo("LightType##component", typeNames[(int)type]))
        {
            for (int i = 0; i < 3; i++)
                if (ImGui::Selectable(typeNames[i]))
                    t.setType((LIGHT)i);
            ImGui::EndCombo();
        }
        ImGui::Text("Light Intensity");
        ImGui::Separator();
        float intensity = t.intensity;
        ImGui::DragFloat("Intensity",&intensity);
        t.setIntensity(intensity);
        ImGui::Text("Ambient Light Color");
        ImGui::Separator();
        glm::vec3 amb = t.ambient;
        float acol[3] = { amb.x,amb.y,amb.z };
        ImGui::ColorEdit3("##Ambient Light Color",acol);
        t.ambientLigting(acol[0],acol[1],acol[2]);
       
        if (type == LIGHT::DIRECTIONAL)
        {
            glm::vec3 ang = t.lightDirection;
            float lang[3] = {ang.x,ang.y,ang.z};
            std::string names[3] = { "x","y","z"};
            ImGui::Text("Directional Light Angles");
            ImGui::Separator();
            dragFloat3(names,lang,80);
            //ImGui::DragFloat3("xyz Directional Direction", lang);
            t.setDirection(lang[0],lang[1],lang[2]);
        }
        if (type == LIGHT::POINT)
        {
            glm::vec3 ptConst = t.pointLightConstants;

            ImGui::Text("Point Light Constants");
            float k[3] = { ptConst.x,ptConst.y,ptConst.z };
            std::string names[3] = { "kC","kL","kQ" };
            ImGui::Separator();
            dragFloat3(names, k, 80);
            t.setPointLightConst(k[0],k[1],k[2]);
            ImGui::TextColored(ImVec4(0.5f,0.5f,0.0f,1),"Point Light Radius:%f",t.getPointLightRadius());


        }
        
        ImGui::Text("Light Color");
        ImGui::Separator();
        float color[3] = { t.lightColor.x,t.lightColor.y,t.lightColor.z};
        ImGui::ColorEdit3("##Light Color",color);
        t.lightColor = glm::vec3(color[0],color[1],color[2]);
        

    }
}
void InspectorWindow::DrawScriptComponent(Entity selected)
{
    if (!selected.signature->test(m_BehaviourComp_pose))
        return;
    if (ImGui::CollapsingHeader("ScriptableComponent", ImGuiTreeNodeFlags_DefaultOpen))
    {
        std::shared_ptr<Behaviour> bh = m_Scene.GetComponent<BehaviourComponent>(selected).behaviour;
        ReflectionUIHandler uiHandle;
        bh->_draw_data(uiHandle);
    }
}
InspectorWindow::InspectorWindow(Scene& m_Scene, std::shared_ptr<ECS> ecs) :m_Scene(m_Scene)
{
    m_ECS = ecs;

    m_Transform_pose = m_ECS->GetComponentBitPose<Transform>();
    m_Descriptor_pose = m_ECS->GetComponentBitPose<EntityDescriptor>();
    m_Mesh_pose = m_ECS->GetComponentBitPose<Mesh>();
    m_RigidBody3D_pose = m_ECS->GetComponentBitPose<physics::RigidBody3D>();
    m_SkeletalMesh_pose = m_ECS->GetComponentBitPose<SkeletalMesh>();
    m_AnimationController_pose = m_ECS->GetComponentBitPose<AnimationController>();
    m_BehaviourComp_pose = m_ECS->GetComponentBitPose<BehaviourComponent>();
    m_Lights_pose = m_ECS->GetComponentBitPose<Lights>();
    m_Material_pose = m_ECS->GetComponentBitPose<Material>();
}
void InspectorWindow::Draw(std::shared_ptr<SceneHierarchyWindow> sceneHierarchy)
{
    ImGui::Begin("properties");
    if (sceneHierarchy->selected.size() > 0)
    {

        Entity selected =*(sceneHierarchy->selected.begin());
        
        if (selected != INVALID_ENTITY)
        {
           
            DrawTransformComponent(selected);
            
            DrawLightComponent(selected);

            DrawScriptComponent(selected);
        }
      
    }
    ImGui::End();


}
