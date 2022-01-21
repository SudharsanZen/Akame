#include"Core/Editor/SettingsWindow/LightsAndShadows.h"
#include"glad/glad.h"
#include"glfw/glfw3.h"
#include"Core\Input.h"
#include<sstream>
#include"Components/EntityDescriptor.h"
#include"Rendering/System/LightSystem.h"
#include"Rendering/System/RenderingSystem.h"
#include"Rendering/System/DeferredPipeline.h"
#include"Rendering/System/PSSMFBO.h"
#include"Core/Debug/Debug.h"
#include"Animation/AnimationControllerSystem.h"
#include"Core/Editor/PropertiesWindow/InspectorWindow.h"
#include"Core/Editor/SceneHierarchyWindow/SceneHierarchyWindow.h"
#include"Core/Editor/ViewPort/ViewPortWindow.h"

#include<imGui\backends\imgui_impl_glfw.h>
#include<imGui\backends\imgui_impl_opengl3.h>

LightAndShadowConfig::LightAndShadowConfig(Scene& scene) :m_scene(scene)
{
	fbo = std::vector<FrameBuffer>(m_scene.lightSys->dirLightSpace.size());
}

void LightAndShadowConfig::Draw()
{
	ImGui::Begin("Shadows:");

	if (ImGui::CollapsingHeader("Directional Shadow", ImGuiTreeNodeFlags_DefaultOpen))
	{

		static float maxz = 1000;
		static float minz = 100;
		ImGui::DragScalar("ShadowDist", ImGuiDataType_Float, &m_scene.lightSys->shadowDistance, 1, &minz, &maxz, "%f");

		ImGui::SliderFloat("log-uniform lambda", &m_scene.lightSys->lambda, 0.0f, 1.0f, "ratio = %.3f");
		ImVec2 pos = ImGui::GetCursorScreenPos();
		static float maxx=2, minx =1 ;
		static float maxy=2, miny =1 ;
		ImGui::DragScalar("x multiplier:", ImGuiDataType_Float, &LightSystem::pssmXMultiplier, 1, &minx, &maxx, "%f");
		ImGui::DragScalar("y multiplier:", ImGuiDataType_Float, &LightSystem::pssmYMultiplier, 1, &miny, &maxy, "%f");
		//pass the texture of the FBO
		//window.getRenderTexture() is the texture of the FBO
		//the next parameter is the upper left corner for the uvs to be applied at
		//the third parameter is the lower right corner
		//the last two parameters are the UVs
		//they have to be flipped (normally they would be (0,0);(1,1) 
		
		if (ImGui::CollapsingHeader("Shdow Cascades"))
		{
			ShaderManager::GetShader("PSSM")->useShaderProgram();
			std::vector<unsigned int> texList;
			for (int i = 0; i < m_scene.lightSys->dirLightSpace.size(); i++)
			{
				glDisable(GL_CULL_FACE);
				fbo[i].Bind();
				ShaderManager::GetShader("PSSM")->setUniformInteger("layer", i);
				m_scene.lightSys->dir_sMap.useTextureArray(0);
				glBindVertexArray(Mesh::VAO);
				fbo[i].quadMesh.renderMesh();
				glBindVertexArray(0);
				fbo[i].unBind();
				glEnable(GL_CULL_FACE);
				texList.push_back(fbo[i].getColorBuffer());
			}
			int div = 2, res = 512 / 3;
			for (int i = 0; i < texList.size(); i++)
			{

				float size = res;
				float gapx = 5.0f * (i % div);
				float gapy = 5.0f * ((i / div));
				float dispY = (i / div) * (size);
				ImVec2 pos = ImGui::GetCursorScreenPos();
				float x = pos.x + (size) * ((i) % div);
				float y = pos.y + dispY;
				unsigned int tex = texList[i];
				ImGui::GetWindowDrawList()->AddImage(
					(void*)tex,
					ImVec2(ImVec2(x + gapx, y + gapy)),
					ImVec2(x + size + gapx,
						y + size + gapy), ImVec2(0, 1), ImVec2(1, 0));
			}
		}


	}
	ImGui::End();
}
