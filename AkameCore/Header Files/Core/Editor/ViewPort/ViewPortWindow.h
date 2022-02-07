#pragma once

#include"Core/Scene.h"
#include"Rendering/Texture.h"
#include"Rendering/FrameBuffer.h"
#include"Assets/ShaderManager.h"
#include"Components/Physics/RigidBody3D.h"
struct ImGuiIO;
class ViewPortWindow
{
private:
	Camera cam;
	FrameBuffer sceneFrame;
	Scene& m_scene;
	ImGuiIO& m_io;
	float X = -45, Y = -135;
	float camSpeed = 10;
	float m_window_height, m_window_width;
	bool open=true;
	bool closable = false;
	friend class Exporter;
	friend class Editor;
public:
	std::string windowName;
	AKAME_API ViewPortWindow(Scene& scene, ImGuiIO& io, bool closable = true);

	AKAME_API void Draw(float deltaTime);
	AKAME_API void setFrameBufferHeight(float height,float width);
	AKAME_API void flyCamScene(Camera& cam, float deltaTime,bool capture);
};