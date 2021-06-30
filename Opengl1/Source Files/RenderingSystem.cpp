#include"System.h"
#include"Mesh.h"
#include"Material.h"
#include "ECS.h"
#include "Systems/RenderingSystem.h"
#include"Camera.h"
void RenderingSystem::GroupEntityWithCommonShader(std::shared_ptr<ECS> ecs)
{
	emptyDrawList();
	for (auto const& ent : entities)
	{
		std::string shdName=ecs->GetComponent<Material>(ent).SHADER_NAME;
		if (drawList.find(shdName) == drawList.end())
		{
			drawList["ERROR"].push_back(ent);
		}
		else
			drawList[shdName].push_back(ent);
	}
}
void RenderingSystem::Run(std::shared_ptr<ECS> ecs, Camera& cam)
{
	GroupEntityWithCommonShader(ecs);

	for (auto const& entList : drawList)
	{
		//get the shader from the shadermanager with the shader name from the map entlist
		std::shared_ptr<Shader> shader=ShaderManager::GetShader(entList.first);
	
		shader->useShaderProgram();
		for (auto const& ent : entList.second)
		{
			Transform& t = ecs->GetComponent<Transform>(ent);
			Mesh& mesh = ecs->GetComponent<Mesh>(ent);
			ecs->GetComponent<Material>(ent).use(t,lightPose,cam.transform.position,shader);
			mesh.renderMesh();
		}


	}
}

void RenderingSystem::emptyDrawList()
{
	//initialize the draw list with empty vectors for each shader name
	for (auto const& pair : ShaderManager::shaderList)
	{
		drawList[pair.first] = std::vector<Entity>();
	}
}
