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
	ViewPortWindow(Scene& scene,ImGuiIO& io,bool closable=true) :m_scene(scene), cam(60, 1, 0.1f, 1000),m_io(io)
	{
		this->closable = closable;
		cam.transform.SetGlobalPosition(glm::vec3(5, 5, 5));
		//cam.transform.SetLocalRotation(Quaternion::rotationAroundAxisVector(-135, worldUp));
		//cam.transform.SetLocalRotation(Quaternion::rotationAroundAxisVector(-45, cam.transform.right()) * cam.transform.GetGlobalRotation());
	}

	void Draw(float deltaTime);
	void setFrameBufferHeight(float height,float width);
	void flyCamScene(Camera& cam, float deltaTime,bool capture);
};