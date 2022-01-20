#pragma once

#include"Core/Scene.h"
#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;

class SceneHierarchyWindow
{
	friend class Editor;
	friend class InspectorWindow;
	std::set<Entity> selected;
	Scene& m_Scene;
	bool viewDebugInfo;
	
	float m_menuPosex, m_menuPosey;
	void Draw();
	void DrawNode(Transform const& t, EntityDescriptor& edt, ImGuiTreeNodeFlags const& base_flags);
public:
	SceneHierarchyWindow(Scene& s) :m_Scene(s)
	{
	}
	
};

