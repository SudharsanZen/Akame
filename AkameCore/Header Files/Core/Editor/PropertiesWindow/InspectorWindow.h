#pragma once

#include"Core/Scene.h"
#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
#include"Components/Physics/RigidBody3D.h"
struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;
class AKAME_API InspectorWindow
{
	Scene& m_Scene;
	std::shared_ptr<ECS> m_ECS;

	ComponentBitPosition m_Transform_pose;
	ComponentBitPosition m_Descriptor_pose;
	ComponentBitPosition m_Mesh_pose;
	ComponentBitPosition m_RigidBody3D_pose;
	ComponentBitPosition m_SkeletalMesh_pose;
	ComponentBitPosition m_AnimationController_pose;
	ComponentBitPosition m_BehaviourComp_pose;
	ComponentBitPosition m_Lights_pose;
	ComponentBitPosition m_Material_pose;
	
	void DrawTransformComponent(Entity selected);
	void DrawLightComponent(Entity selected);;
	void DrawScriptComponent(Entity selected);
public:
	InspectorWindow(Scene& m_Scene, std::shared_ptr<ECS> ecs);

	void Draw(std::shared_ptr<SceneHierarchyWindow>);
};

