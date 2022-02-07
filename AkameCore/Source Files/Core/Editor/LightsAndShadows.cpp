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
#include"ECS.h"
LightAndShadowConfig::LightAndShadowConfig(Scene& scene) :m_scene(scene)
{
	fbo = std::vector<FrameBuffer>(m_scene.lightSys->dirLightSpace.size());
}

void LightAndShadowConfig::Draw()
{
	ImGui::Begin("Shadows:");
	bool clicked = ImGui::Button("Reload Shaders", ImVec2(200.0f, 25.0f));

	if (clicked)
	{

		ShaderManager::releaseAllShaders();
		ShaderManager::loadAllShaders();
		m_scene.renderSys->attachAllBuiltInSRP();

	}
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
				glViewport(0, 0, fbo[i].getFrameSize().x, fbo[i].getFrameSize().y);
				fbo[i].quadMesh.renderMesh();
				glBindVertexArray(0);
				fbo[i].unBind();
				glEnable(GL_CULL_FACE);
				texList.push_back(fbo[i].getColorBuffer());
			}
			int div = 2;
			float size = ImGui::GetWindowWidth()/2.0f;
			int maxCol = glm::ceil(ImGui::GetWindowWidth()/(size+0.1f*size));
			
			if (ImGui::BeginTable("DirectionalSS",maxCol))
			{
				for (int i = 0; i < texList.size(); i++)
				{
					if (i % (maxCol) == 0)
					{
						ImGui::TableNextRow();
					}
					ImGui::TableSetColumnIndex(i%maxCol);
					GLuint t = texList[i];
					ImGui::Image((ImTextureID)t,ImVec2(size,size),ImVec2(0,0),ImVec2(1,1));
				
				}
				ImGui::EndTable();
			}
		}


	}
	ImGui::End();
}
