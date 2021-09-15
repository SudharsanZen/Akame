#include"glad/glad.h"
#include"System.h"
#include"Components/Rendering/Mesh.h"
#include"Components/Rendering/Material.h"
#include "ECS.h"
#include"Rendering/System/LightSystem.h"
#include "Rendering/System/RenderingSystem.h"
#include"GLFW/glfw3.h"
#include<sstream>
#include"Rendering/Camera.h"
#include"Rendering/System/DeferredPipeline.h"

//returns the point where the ray intersects the plane
glm::vec3 rayPlaneIntersectionPoint(glm::vec3 rayOrigin,glm::vec3 rayDir,glm::vec3 planeNormal,glm::vec3 planePoint)
{
	float length = glm::dot((planePoint-rayOrigin),planeNormal) / glm::dot(rayDir,planeNormal);
	return rayOrigin + rayDir * length;

}
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


}

void RenderingSystem::attachAllBuiltInSRP()
{
	ShaderManager::AttachShaderPipeline<DeferredPipeline>("DEFERRED");
	ShaderManager::AttachShaderPipeline<RM_SKY_BOXPipeline>("SPHERE");
}

RenderingSystem::RenderingSystem()
{
	
	
	emptyDrawList();
	
	//intialize global uniform buffer for storing projection and view matrix
	glGenBuffers(1, &uboMatrixBufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrixBufferID);
	glBufferData(GL_UNIFORM_BUFFER, 2 * mat4Size, NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrixBufferID, 0, 2 * mat4Size);
	attachAllBuiltInSRP();
}

void RenderingSystem::Run(Camera& cam)
{
	std::shared_ptr<LightSystem> lsys=lightsystem.lock();

	GroupEntityWithCommonShader(ecs.lock());
	
	updateUniformBuffer(cam);
	
	std::shared_ptr<ECS> E = ecs.lock();
	std::shared_ptr<Shader> shader;

	//directional shadow maps 
	shader=ShaderManager::GetShader("SHADOW_DIRECTIONAL");
	shader->useShaderProgram();
	lsys->BindDirectionalLightShadowMap(shader,cam);
		RenderAllMesh(shader,cam);
	lsys->unBindDirectionalShadowMap();
	//directional shadow maps done-------------------------------------------------------------
	
	glViewport(0, 0, width, height);
	RenderQueue("GEOMETRY",cam);
	RenderQueue("OVERLAY",cam);
	



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
	//call all the call back function of all the registered ShaderRenderPipline classes for each shader
	for (auto& SRP : ShaderManager::shaderRenderPipeline)
	{
		SRP.second->WindowsResizeCallBacks(height,width);
	}
}

void RenderingSystem::viewPortSize(int x, int y, int height, int width)
{
	glViewport(0, 0, width, height);
}

void RenderingSystem::depthTestOn(bool state)
{
	if (state)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}



//render all entities under the given shader
void RenderingSystem::RenderAllEntitiesWithShader(std::string SHADERNAME,Camera cam)
{
	
	std::shared_ptr<ECS> E = ecs.lock();
	std::shared_ptr<Shader> shader = ShaderManager::GetShader(SHADERNAME);
	shader->useShaderProgram();
	auto itr = drawList.find(SHADERNAME);
	if (itr != drawList.end())
	{
		if (drawList[SHADERNAME].size() == 0)
			return;
		glEnable(GL_BLEND);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		//check if a pipeline is registered in the name of the shader or not 
		bool pipeReg = ShaderManager::checkForPipeline(SHADERNAME);
		std::shared_ptr<ShaderRenderPipeline> shdPipe;
		if (pipeReg)
		{
			//if registered then call the pre render call back function
			shdPipe = ShaderManager::shaderRenderPipeline[SHADERNAME];
			shdPipe->OnPreRender(shader, this, cam);
		}

		//loop through all entities under the shader name and render it
		for (auto const& ent : drawList[SHADERNAME])
		{
			Transform& t = E->GetComponent<Transform>(ent);
			Mesh& mesh = E->GetComponent<Mesh>(ent);
			E->GetComponent<Material>(ent).use(t, glm::vec3(0), cam.transform.GetGlobalPosition(), shader);
			mesh.renderMesh();
		}

		if (pipeReg)
		{
			//call the post render call back function registered under this shader
			shdPipe->OnPostRender(shader,this,cam);
		}
	}
	
}
//render all object under the given shader
void RenderingSystem::RenderAllMesh(std::shared_ptr<Shader> shader,Camera cam)
{
	std::shared_ptr<ECS> E = ecs.lock();

	for (auto const& entList : drawList)
	{

		//exclude DEFERRED material from being rendered here
		for (auto const& ent : entList.second)
		{
			Transform& t = E->GetComponent<Transform>(ent);
			Mesh& mesh = E->GetComponent<Mesh>(ent);
			glm::mat4 tmat = t.transformMatrix();
			shader->setUniformMat4fv("transform", 1, glm::value_ptr(tmat));
			mesh.renderMesh();
		}


	}
}
//renders all entities with all shaders registered under this queue type
void RenderingSystem::RenderQueue(std::string QUEUENAME,Camera cam)
{
	auto const& sQ = ShaderManager::shaderQueues[QUEUENAME];
	for (int i = 0; i < sQ.size(); i++)
	{
		RenderAllEntitiesWithShader(sQ[i].second, cam);
	}
}

