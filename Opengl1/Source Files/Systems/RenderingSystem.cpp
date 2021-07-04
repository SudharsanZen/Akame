#include"glad/glad.h"
#include"System.h"
#include"Mesh.h"
#include"Material.h"
#include "ECS.h"
#include"Systems/LightSystem.h"
#include "Systems/RenderingSystem.h"
#include"GLFW/glfw3.h"
#include<sstream>
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
void RenderingSystem::setDeferredLightUniforms(std::shared_ptr<ECS> ecs,std::shared_ptr<Shader> shader,Camera &cam)
{
	std::shared_ptr<LightSystem> ltSys = lightsystem.lock();
	
	std::vector<Entity> &direc = ltSys->lightsList[LIGHT::DIRECTIONAL];
	int NR_DIR_L =direc.size();
	
	std::vector<Entity> &point = ltSys->lightsList[LIGHT::POINT];
	int NR_POINT_L = point.size();

	//setting all the directional light variables
	shader->setUniformInteger("NR_DIR_L",NR_DIR_L);
	auto dirVarName = [](std::string structVar, int index)
	{
		std::stringstream str;
		str << "DIR_L["<< index<< "]." + structVar;
		return str.str();
	};
	for (int i = 0; i < NR_DIR_L; i++)
	{
		Lights& l = ecs->GetComponent<Lights>(direc[i]);
		shader->setUniformVec3( dirVarName("lightDir",i) ,l.lightDirection);
		shader->setUniformVec3( dirVarName("lightColor",i) ,l.lightColor);
		shader->setUniformVec3( dirVarName("ambient",i) ,l.ambient);
		shader->setUniformFloat( dirVarName("intensity",i) ,l.intensity);
		shader->setUniformVec3( dirVarName("viewDir",i) ,cam.transform.forward());

	}
	
	shader->setUniformInteger("NR_POINT_L", NR_POINT_L);
	//setting all the Point light variables
	auto pointVarName = [](std::string structVar, int index)
	{
		std::stringstream str;
		str << "POINT_L[" << index << "]." + structVar;
		return str.str();
	};


	

	for (int i = 0; i < NR_POINT_L; i++)
	{
		Lights& l = ecs->GetComponent<Lights>(point[i]);
		Transform& t = ecs->GetComponent<Transform>(point[i]);
		shader->setUniformVec3(pointVarName("lightPose", i), t.position);
		shader->setUniformVec3(pointVarName("lightColor", i), l.lightColor);
		shader->setUniformVec3(pointVarName("ambient", i), l.ambient);
		shader->setUniformVec3(pointVarName("constants", i), l.pointLightConstants);
		shader->setUniformFloat(pointVarName("intensity", i), l.intensity);

	}


	

}
void RenderingSystem::Run(std::shared_ptr<ECS> ecs, Camera& cam)
{
	GroupEntityWithCommonShader(ecs);
	
	//deferred renderer
	std::shared_ptr<Shader> shader=ShaderManager::GetShader("DEFERRED");
	shader->useShaderProgram();
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_MULTISAMPLE);
	drfb.bindFrameBuffer();
	for (auto const& ent : drawList["DEFERRED"])
	{
		Transform& t = ecs->GetComponent<Transform>(ent);
		Mesh& mesh = ecs->GetComponent<Mesh>(ent);
		ecs->GetComponent<Material>(ent).use(t, lightPose, cam.transform.position, shader);
		mesh.renderMesh();
	}
	drfb.unBindFrameBuffer();
	glDisable(GL_DEPTH_TEST);
	shader = ShaderManager::GetShader("DEFERRED_OUT");
	shader->useShaderProgram();
	setDeferredLightUniforms(ecs,shader,cam);
	drfb.outPutToQaud(cam,shader);




	glEnable(GL_DEPTH_TEST);
	for (auto const& entList : drawList)
	{
		//get the shader from the shadermanager with the shader name from the map entlist
		shader=ShaderManager::GetShader(entList.first);
		if (entList.first == "DEFERRED")
			continue;
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

void RenderingSystem::updateFrameBufferSize(int height, int width)
{
	fb.updateTextureSize(height,width);
	drfb.updateBufferSize(height,width);
}
