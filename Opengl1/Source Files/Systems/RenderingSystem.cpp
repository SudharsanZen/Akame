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

#define DIR_MAP_SIZE 8192
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
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

}

RenderingSystem::RenderingSystem():dir_sMap(DIR_MAP_SIZE,DIR_MAP_SIZE)
{
	lightPose = glm::vec3(10, 4, 10);
	
	emptyDrawList();
	quad.CreateMesh(BasicShapes::quadVert,BasicShapes::quadIndices);
	//intialize global uniform buffer for storing projection and view matrix
	glGenBuffers(1, &uboMatrixBufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrixBufferID);
	glBufferData(GL_UNIFORM_BUFFER, 2 * mat4Size, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrixBufferID, 0, 2 * mat4Size);

}

void RenderingSystem::Run(std::shared_ptr<ECS> ecs, Camera& cam)
{
	std::shared_ptr<LightSystem> lsys=lightsystem.lock();
	updateUniformBuffer(cam);
	GroupEntityWithCommonShader(ecs);
	std::shared_ptr<Shader> shader;
	glm::mat4 dirLightSpace=glm::mat4(0);
	//get the shader from the shadermanager with the shader name from the map entlist
	if (lsys->drVector.size())
	{
		glCullFace(GL_FRONT);
		glViewport(0, 0, DIR_MAP_SIZE, DIR_MAP_SIZE);
		auto const& drLight = lsys->drVector[0];

		glm::vec3 forward = glm::normalize(drLight.lightDir);
		glm::vec3 pose = -forward * 36.0f;
		glm::vec3 up = normalize(glm::cross(forward, -worldRight));

		shader = ShaderManager::GetShader("SHADOW_DIRECTIONAL");
		dir_sMap.bindShadowBuffer();
		shader->useShaderProgram();
		glm::mat4 viewMat = glm::lookAt(pose, pose + (forward * 10.0f), up);
		
		float boxSize = 50;

		glm::mat4 projOrtho = glm::ortho(-boxSize, boxSize, -boxSize, boxSize, 0.1f, 100.0f);
		shader->setUniformMat4fv("projMat", 1, glm::value_ptr(projOrtho));
		shader->setUniformMat4fv("view", 1, glm::value_ptr(viewMat));
		dirLightSpace = projOrtho*viewMat;
		for (auto const& entList : drawList)
		{

			//exclude DEFERRED material from being rendered here
			for (auto const& ent : entList.second)
			{
				Transform& t = ecs->GetComponent<Transform>(ent);
				Mesh& mesh = ecs->GetComponent<Mesh>(ent);
				glm::mat4 tmat = t.transformMatrix();
				shader->setUniformMat4fv("transform", 1, glm::value_ptr(tmat));
				mesh.renderMesh();
			}


		}
		dir_sMap.unBindShadowBuffer();
		glDisable(GL_DEPTH_TEST);
		glViewport(0, 0, width, height);

		glCullFace(GL_BACK);
	}
	//directional shadow maps done-------------------------------------------------------------





	glViewport(0,0,width,height);
	//deferred renderer-----------------------------------------------------------------------
		shader=ShaderManager::GetShader("DEFERRED");
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
			
			//render final to quad
			glDisable(GL_DEPTH_TEST);//disable depth to remove quad from depth calulations
			drfb.setUpShader(cam, lightsystem.lock());
			dir_sMap.useDepthTexture(6);
			drfb.set4x4Matrixfv("lightSpaceMat",dirLightSpace);
			drfb.outPutToQaud();
	//deferred renderer end-------------------------------------------------------------------


	//forward renderer start------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	for (auto const& entList : drawList)
	{
		//get the shader from the shadermanager with the shader name from the map entlist
		shader=ShaderManager::GetShader(entList.first);
		//exclude DEFERRED material from being rendered here
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

	//forward renderer end--------------------------------------------------------------------
	
	//render directional shadows--------------------------------------------------------------
	




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
