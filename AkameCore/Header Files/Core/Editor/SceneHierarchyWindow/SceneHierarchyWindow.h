#pragma once

#include"Core/Scene.h"
#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
#include"Core/Editor/AkameWindowBase.h"
struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;

class SceneHierarchyWindow:public AkameWindowBase
{
	friend class Editor;
	friend class Exporter;
	friend class InspectorWindow;
	std::set<Entity> selected;
	Scene& m_scene;
	bool viewDebugInfo;
	
	float m_menuPosex, m_menuPosey;
	AKAME_API void Draw();
	AKAME_API void DrawNode(Transform const& t, EntityDescriptor& edt, ImGuiTreeNodeFlags const& base_flags);
public:
	AKAME_API Entity GetFirstSelection();
	AKAME_API SceneHierarchyWindow(Scene& s);
	
};

