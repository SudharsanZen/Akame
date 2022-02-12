#include"Rendering/System/RenderingSystem.h"
#include"Core/Editor/ViewPort/ViewPortWindow.h"
#pragma warning(push, 0)
#pragma warning( disable : 26495)
#pragma warning( disable : 6031)
#pragma warning( disable : 26812)
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#pragma warning(pop)
#include"Core/Input.h"
#include"Core/Window.h"
#include"ECS.h"

ViewPortWindow::ViewPortWindow(Scene& scene, ImGuiIO& io, bool closable) :m_scene(scene), cam(60, 1, 0.1f, 1000), m_io(io)
{
	this->closable = closable;
	cam.transform.SetGlobalPosition(glm::vec3(5, 5, 5));
	m_window_height = 0;
	m_window_width = 0;
	//cam.transform.SetLocalRotation(Quaternion::rotationAroundAxisVector(-135, worldUp));
	//cam.transform.SetLocalRotation(Quaternion::rotationAroundAxisVector(-45, cam.transform.right()) * cam.transform.GetGlobalRotation());
}
void ViewPortWindow::Draw(float deltaTime)
{
	

	//call this in this same order
	
	
	sceneFrame.Bind();

	sceneFrame.unBind();
	
	bool renderStuff=false;
	if (closable)
		renderStuff = ImGui::Begin(windowName.c_str(), &open);
	else
		renderStuff = ImGui::Begin(windowName.c_str());

	if (renderStuff && open)
	{
		
		flyCamScene(cam, deltaTime, ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow));
		
		ImVec2 size = ImGui::GetWindowSize();
		
		m_window_height = size.y;
		m_window_width = size.x;
		setFrameBufferHeight(m_window_height, m_window_width);
		m_scene.renderSys->updateFrameBufferSize(static_cast<int>(m_window_height), static_cast<int>(m_window_width));
		cam.setAspectRation((float)m_window_width / (float)m_window_height);
		m_scene.renderSys->height = static_cast<int>(m_window_height);
		m_scene.renderSys->width = static_cast<int>(m_window_width);
		ImVec2 pose = ImGui::GetCursorScreenPos();
		ImGui::GetWindowDrawList()->AddImage((void*)static_cast<int64_t>(sceneFrame.getColorBuffer()), pose,
			ImVec2(pose.x + m_window_width, pose.y + m_window_height),
			ImVec2(0, 1), ImVec2(1, 0));
		m_scene.renderSys->Run(cam, sceneFrame.framebuffer);
		
		
		
	}
	ImGui::End();
	sceneFrame.unBind();
	
}

void ViewPortWindow::setFrameBufferHeight(float height, float width)
{
	sceneFrame.updateTextureSize((int)height,(int)width);
}

void ViewPortWindow::flyCamScene(Camera& cam, float deltaTime,bool capture)
{
	static bool init = false;

	float speed = camSpeed*deltaTime;
	
	if (capture)
	{
		if (m_io.KeyShift)
		{
			speed *= 5;
		}
		if (m_io.KeysDown[KEY_W])
		{
			cam.setCameraPosition(cam.getCameraPosition() + cam.transform.forward() * speed);

		}
		if (m_io.KeysDown[KEY_S])
		{
			cam.setCameraPosition(cam.getCameraPosition() - cam.transform.forward() * speed);

		}
		if (m_io.KeysDown[KEY_A])
		{
			cam.setCameraPosition(cam.getCameraPosition() - cam.transform.right() * speed);

		}
		if (m_io.KeysDown[KEY_D])
		{
			cam.setCameraPosition(cam.getCameraPosition() + cam.transform.right() * speed);

		}
		ImVec2 del = m_io.MouseDelta;


		if (m_io.MouseDown[1] || !init)
		{
			init = true;
			X += -del.y * 0.5f;
			Y += -del.x * 0.5f;
			//if (abs(Y) >= 360.0f)
				//Y =Y+((Y>0)?-1:1)*360.0f;
			Y = abs(Y) >= 360.0f ? 0 : Y;
			X = abs(X) >= 360.0f ? 0 : X;

		}
	}
	Quaternion initRot = cam.transform.GetGlobalRotation();
	cam.transform.SetGlobalRotation(Quaternion::rotationAroundAxisVector(Y, worldUp));
	cam.transform.SetGlobalRotation(Quaternion::rotationAroundAxisVector(X, cam.transform.right()) * cam.transform.GetGlobalRotation());
	Quaternion newRot = cam.transform.GetGlobalRotation();
	deltaTime = (deltaTime>0.02f)?0.02f:deltaTime;
	cam.transform.SetGlobalRotation(glm::slerp(initRot.quaternion, newRot.quaternion, deltaTime * 20.0f));

	
}
