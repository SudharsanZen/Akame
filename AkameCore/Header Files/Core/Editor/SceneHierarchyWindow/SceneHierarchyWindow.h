#pragma once
#include"Core/Window.h"
#include"Core/Scene.h"
#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
struct ImGuiIO;
typedef int ImGuiTreeNodeFlags;

class SceneHierarchyWindow
{
	friend class Editor;

	std::set<Entity> selected;
	Scene& scene;
	bool viewDebugInfo;
	void Draw();
	void DrawNode(Transform const& t, EntityDescriptor& edt, ImGuiTreeNodeFlags const& base_flags);
public:
	SceneHierarchyWindow(Scene& s) :scene(s)
	{
	}
	
};

