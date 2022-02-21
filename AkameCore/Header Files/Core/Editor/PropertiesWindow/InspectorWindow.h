#pragma once

#include"Core/Scene.h"
#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
#include"Components/Physics/RigidBody3D.h"
#include"Core/Editor/AkameWindowBase.h"
struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;
class InspectorWindow:public AkameWindowBase
{
	Scene& m_scene;
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
	
	AKAME_API void DrawTransformComponent(Entity selected);
	AKAME_API void DrawLightComponent(Entity selected);;
	AKAME_API void DrawScriptComponent(Entity selected);
public:
	AKAME_API InspectorWindow(Scene& m_scene, std::shared_ptr<ECS> ecs);

	AKAME_API void Draw(std::shared_ptr<SceneHierarchyWindow>);
};

