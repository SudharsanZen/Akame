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
#include"Rendering/System/DefaultRenderingPipeline.h"
#include"Core/Debug/Debug.h"
#include"Animation/SkinnedRendererPipeline.h"
//returns the point where the ray intersects the plane
glm::vec3 rayPlaneIntersectionPoint(glm::vec3 rayOrigin,glm::vec3 rayDir,glm::vec3 planeNormal,glm::vec3 planePoint)
{
	float length = glm::dot((planePoint-rayOrigin),planeNormal) / glm::dot(rayDir,planeNormal);
	return rayOrigin + rayDir * length;

}
void RenderingSystem::GroupEntityWithCommonShader()
{
	std::shared_ptr<ECS> e = ecs.lock();
	emptyDrawList();
	for (auto const& ent : entities)
	{
		Material &m= e->GetComponent<Material>(ent);
		std::string shdName = m.SHADER_NAME;
		if (drawList.find(shdName) == drawList.end())
		{
			drawList["ERROR"][0].push_back(ent);
		}
		else
		{
			drawList[shdName][*m.materialID].push_back(ent);
		}
	
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
	ShaderManager::shaderRenderPipeline.clear();
	ShaderManager::AttachShaderPipeline<DeferredPipeline>("DEFERRED",height,width);
	ShaderManager::AttachShaderPipeline<RM_SKY_BOXPipeline>("SPHERE");
	ShaderManager::AttachShaderPipeline<DefaultRenderingPipeline>("DEFAULT");
	ShaderManager::AttachShaderPipeline<SkeletalMeshRenderingPipeline>("SkinnedMeshRenderer");
}

RenderingSystem::RenderingSystem()
{
	glGenBuffers(1,&transformUBO);
	glBindBuffer(GL_UNIFORM_BUFFER,transformUBO);
	glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE,&maxUBOSize);

	ENGINE_CORE_INFO("MAX UNIFORM BLOCK SIZE: {0:d}",maxUBOSize);
	maxTransformUploadable = maxUBOSize/sizeof(glm::mat4);
	glBufferData(GL_UNIFORM_BUFFER, maxTransformUploadable * mat4Size, NULL, GL_STATIC_DRAW);
	int off;
	glGetIntegerv(GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT,&off);
	ENGINE_CORE_INFO("MAX TRANSFORM UPLOADABLE: {0:d}", maxTransformUploadable);
	ENGINE_CORE_INFO("UNIFORM BUFFER OFFSET ALIGNMENT: {0:d}", off);
	emptyDrawList();
	
	//intialize global uniform buffer for storing projection and view matrix
	glGenBuffers(1, &uboMatrixBufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, uboMatrixBufferID);
	glBufferData(GL_UNIFORM_BUFFER, 2 * mat4Size, NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, uboMatrixBufferID, 0, 2 * mat4Size);
	attachAllBuiltInSRP();
}

void RenderingSystem::Run(Camera& cam, unsigned int frameBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);
	if (Mesh::needsUpdate)
	{
		Mesh::needsUpdate = false;
		Mesh::setupMesh();
	}
	meshArray = (ComponentArray<Mesh>*)(ecs.lock()->componentManager.compIDtoArray[typeid(Mesh).name()].get());
	transformArray = (ComponentArray<Transform>*)(ecs.lock()->componentManager.compIDtoArray[typeid(Transform).name()].get());

	std::shared_ptr<LightSystem> lsys=lightsystem.lock();
	if (m_update_Event)
	{
		m_update_Event = false;
		GroupEntityWithCommonShader();
	}
	updateUniformBuffer(cam);
	
	std::shared_ptr<ECS> E = ecs.lock();
	std::shared_ptr<Shader> shader;

	
	//directional shadow maps for non-skeletal meshes

	shader=ShaderManager::GetShader("SHADOW_DIRECTIONAL");
	lsys->dir_sMap.bind();
	lsys->dir_sMap.clearBuffer();
	lsys->BindDirectionalLightShadowMap(shader,cam);
		RenderAllMesh(shader,cam);
	//directional shadow maps for skeletal meshes
	shader = ShaderManager::GetShader("SK_DIR_SHADOW");
	lsys->BindDirectionalLightShadowMap(shader, cam);
		glBindVertexArray(SkeletalMesh::VAO);
			animSys.lock()->RenderShadows(this,shader,cam);
		glBindVertexArray(0);
	lsys->unBindDirectionalShadowMap(frameBuffer);
	//directional shadow maps done-------------------------------------------------------------
	
	glViewport(0, 0, width, height);
	RenderQueue("GEOMETRY",cam,frameBuffer);
	//render all skeletal mesh-----------------------------
	glBindVertexArray(SkeletalMesh::VAO);
	animSys.lock()->renderMeshes(this,cam);
	glBindVertexArray(0);
	//-----------------------------------------------------
	RenderQueue("OVERLAY",cam,frameBuffer);

	
	ShaderManager::GetShader("LINES_DEBUG")->useShaderProgram();
	glDisable(GL_DEPTH_TEST);
	if(!editorMode)
		Debug::updateBufferContent();
	Debug::DrawDebug();
	if(!editorMode)
		Debug::FlushDebugInformation();
	glEnable(GL_DEPTH_TEST);
	
	glBindFramebuffer(GL_FRAMEBUFFER,0);

}

void RenderingSystem::emptyDrawList()
{
	//initialize the draw list with empty vectors for each shader name
	for (auto const& pair : ShaderManager::shaderList)
	{
		drawList[pair.first] = std::map<unsigned long long ,std::vector<Entity>>();
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
void RenderingSystem::RenderAllEntitiesWithShader(std::string SHADERNAME,Camera cam, unsigned int frameBuffer)
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
			shdPipe->OnPreRender(shader, this, cam,frameBuffer);
		}
		e_index tran = transformArray->componentBitPose;;
		e_index me = meshArray->componentBitPose;
		//loop through all entities under the shader name and render it
		glBindVertexArray(Mesh::VAO);
		glBindBufferBase(GL_UNIFORM_BUFFER, 8, transformUBO);
		for (auto const& entMatList : drawList[SHADERNAME])
		{
			auto const& entList = entMatList.second;
			
			
			long int listSize=entMatList.second.size();

			long int row = ceil(float(listSize) / 1000.0f);
			Material &m=E->GetComponent<Material>(entMatList.second[0]);
			m.setUniformsOnce(shader,cam.transform.GetGlobalPosition());
			for (long int r = 0; r < row; r++)
			{
				long long int curr_i = 0;
				std::vector<glm::mat4> matList;
				for (long int i_mat = r*1000; i_mat < (1+r)*1000 && i_mat < entList.size(); i_mat++)
				{
					
					const Entity& ent = entList[i_mat];
					Transform& t = transformArray->GetData((*ent.componentIndex)[tran]);
					matList.push_back(t.transformMat);
				}
				glBufferSubData(GL_UNIFORM_BUFFER,0, mat4Size*matList.size(),&(matList[0][0].x));
				curr_i++;
				curr_i = 0;
				for (long int i = r*1000; i < (r+1)*1000 && i< entList.size(); i++)
				{
					const Entity& ent = entList[i];

					Mesh& mesh = E->GetComponent<Mesh>((*ent.componentIndex)[me], me);
					m.setUniformEveryObject(curr_i, shader);
					mesh.renderMesh();
					curr_i++;
				}
			}
		}
		glBindVertexArray(0);
		if (pipeReg)
		{
			//call the post render call back function registered under this shader
			shdPipe->OnPostRender(shader,this,cam,frameBuffer);
		}
	}
	
}
//render all object under the given shader
void RenderingSystem::RenderAllMesh(std::shared_ptr<Shader> shader,Camera cam)
{
	std::shared_ptr<ECS> E = ecs.lock();
	e_index tran = transformArray->componentBitPose;;
	e_index me = meshArray->componentBitPose;
	unsigned int transformLocation = shader->getUniformLocation("t_index");
	glBindVertexArray(Mesh::VAO);
	glBindBufferBase(GL_UNIFORM_BUFFER, 8, transformUBO);
	
	for (auto const& shaderEntList : drawList)
	{
		
		for (auto const& entMatList : shaderEntList.second)
		{
			auto const& entList = entMatList.second;
			long int listSize = entList.size();
			long int row = ceil(float(listSize) / 1000.0f);
			for (long int r = 0; r < row; r++)
			{
				long long int curr_i = 0;
				std::vector<glm::mat4> matList;
				for (long int i_mat = r * 1000; i_mat < (1 + r) * 1000 && i_mat < listSize; i_mat++)
				{

					const Entity& ent = entList[i_mat];
					Transform& t = transformArray->GetData((*ent.componentIndex)[tran]);
					matList.push_back(t.transformMat);
				}
				glBufferSubData(GL_UNIFORM_BUFFER, 0, mat4Size * matList.size(), &(matList[0][0].x));
				curr_i++;
				curr_i = 0;
				for (long int i = r * 1000; i < (r + 1) * 1000 && i < listSize; i++)
				{
					const Entity& ent = entList[i];

					Mesh& mesh = E->GetComponent<Mesh>((*ent.componentIndex)[me], me);
					shader->setUniformInteger(transformLocation, curr_i);
					mesh.renderMesh();
					curr_i++;
				}
			}
		}
	}
	
	glBindVertexArray(0);
}
//renders all entities with all shaders registered under this queue type
void RenderingSystem::RenderQueue(std::string QUEUENAME,Camera cam, unsigned int frameBuffer)
{
	auto const& sQ = ShaderManager::shaderQueues[QUEUENAME];
	for (int i = 0; i < sQ.size(); i++)
	{
		RenderAllEntitiesWithShader(sQ[i].second, cam,frameBuffer);
	}
}

