#include"glad/glad.h"
#include"System.h"
#include"Mesh.h"
#include"Material.h"
#include "ECS.h"
#include"Systems/RenderingSystem/LightSystem.h"
#include "Systems/RenderingSystem/RenderingSystem.h"
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


/*Call this to update the common global uniforms.
* this updates the Projection matrix and ViewMatrix which is common to all shaders
*/

void RenderingSystem::updateUniformBuffer(Camera& cam)
{
	glm::mat4 projMat = cam.getProjectionMatrix();
	glm::mat4 viewMat = cam.getViewMatrix();
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrixBufferID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, mat4Size, glm::value_ptr(projMat));
	glBufferSubData(GL_UNIFORM_BUFFER, mat4Size, mat4Size, glm::value_ptr(viewMat));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

RenderingSystem::RenderingSystem()
{
	lightPose = glm::vec3(10, 4, 10);
	emptyDrawList();
	//intialize global uniform buffer for storing projection and view matrix
	glGenBuffers(1, &uboMatrixBufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrixBufferID);
	glBufferData(GL_UNIFORM_BUFFER, 2 * mat4Size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrixBufferID, 0, 2 * mat4Size);

}

void RenderingSystem::Run(std::shared_ptr<ECS> ecs, Camera& cam)
{
	updateUniformBuffer(cam);
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
	drfb.outPutToQaud(cam,lightsystem.lock());
	//drfb.drawNormalBuffer();



	glEnable(GL_DEPTH_TEST);

	updateUniformBuffer(cam);
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
