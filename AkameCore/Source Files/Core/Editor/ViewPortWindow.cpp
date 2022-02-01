#include"Rendering/System/RenderingSystem.h"
#include"Core/Editor/ViewPort/ViewPortWindow.h"
#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>
#include"Core/Input.h"
#include"Core/Window.h"
void ViewPortWindow::Draw(float deltaTime)
{
	

	//call this in this same order
	
	
	sceneFrame.Bind();

	sceneFrame.unBind();
	
	bool renderStuff=true;
	if (closable)
		renderStuff = ImGui::Begin(windowName.c_str(), &open);
	else
		renderStuff = ImGui::Begin(windowName.c_str());

	if (renderStuff)
	{
	
		flyCamScene(cam, deltaTime, ImGui::IsWindowFocused(ImGuiFocusedFlags_RootWindow));
		
		ImVec2 size = ImGui::GetWindowSize();
		
		m_window_height = size.y;
		m_window_width = size.x;
		setFrameBufferHeight(m_window_height, m_window_width);
		m_scene.renderSys->updateFrameBufferSize(m_window_height, m_window_width);
		cam.setAspectRation((float)m_window_width / (float)m_window_height);
		m_scene.renderSys->height = m_window_height;
		m_scene.renderSys->width = m_window_width;
		ImVec2 pose = ImGui::GetCursorScreenPos();
		ImGui::GetWindowDrawList()->AddImage((void*)sceneFrame.getColorBuffer(), pose,
			ImVec2(pose.x + m_window_width, pose.y + m_window_height),
			ImVec2(0, 1), ImVec2(1, 0));
		m_scene.renderSys->Run(cam, sceneFrame.framebuffer);
		
		ImGui::End();
		
	}
	sceneFrame.unBind();
	
}

void ViewPortWindow::setFrameBufferHeight(float height, float width)
{
	sceneFrame.updateTextureSize(height,width);
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
	cam.transform.SetGlobalRotation(glm::slerp(initRot.quaternion, newRot.quaternion, deltaTime * 20.0f));

	
}
