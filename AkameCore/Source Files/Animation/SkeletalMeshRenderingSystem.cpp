#include"Rendering/System/ShaderRenderPipeline.h"
#include "Animation/SkeletalMeshRenderingSystem.h"
#include"ECS.h"
SkeletalMeshRenderingSystem::SkeletalMeshRenderingSystem()
{
	needs_update = true;
}

void SkeletalMeshRenderingSystem::OnAddEntity(Entity eid)
{
	needs_update = true;
}

void SkeletalMeshRenderingSystem::OnDestroyEntity(Entity eid)
{
	needs_update = true;
}

void SkeletalMeshRenderingSystem::renderMeshes(RenderingSystem* rendSys, Camera cam)
{
	RenderSkeletalMeshQueue(rendSys, cam);
}

void SkeletalMeshRenderingSystem::RenderSkeletalMeshQueue(RenderingSystem* rendSys,Camera cam)
{
	auto& m_shaderList = ShaderManager::m_shaderQueues["SKINNED"];
	for (int i = 0; i < m_shaderList.size(); i++)
	{
		RenderEntitiesWithShader(rendSys,m_shaderList[i].second, cam);
	}
}

void SkeletalMeshRenderingSystem::RenderEntitiesWithShader(RenderingSystem* rendSys,std::string shaderName, Camera cam)
{
	//if no entity in this system uses the shader given, then just return (nothing to render)
	if (mShaderNameEntityMap.find(shaderName) == mShaderNameEntityMap.end())
	{
		return;
	}

	std::shared_ptr<Shader> skRend = ShaderManager::GetShader(shaderName);
	std::shared_ptr<ECS> e = ecs.lock();
	skRend->useShaderProgram();
	if (ShaderManager::checkForPipeline(shaderName))
	{

		std::shared_ptr<ShaderRenderPipeline> rendPipe = ShaderManager::m_shaderRenderPipeline[shaderName];
		rendPipe->OnPreRender(skRend,rendSys,cam);
		if (ShaderManager::m_shaderList.find(shaderName) == ShaderManager::m_shaderList.end())
		{
			ENGINE_CORE_ERROR("SkeletalMeshRenderingSystem::SHADER NAME is invalid, can't find shader in shaderConf.XML");
			return;
		}
		for (auto& ent : mShaderNameEntityMap[shaderName])
		{

			SkeletalMesh& sk = e->GetComponent<SkeletalMesh>(ent);
			Material& m	= e->GetComponent<Material>(ent);
			m.setUniformOnceSkeletalMesh(skRend, cam);
			//On Entity render call back
			rendPipe->OnEntityRender(skRend, e, cam, ent);
			
			sk.renderMesh();
		}
	}
}

void SkeletalMeshRenderingSystem::RenderShadows(RenderingSystem* rendSys, std::shared_ptr<Shader> skRend, Camera cam)
{

	
	std::shared_ptr<ECS> e = ecs.lock();
	
		for (auto& ent : entities)
		{

			
			SkeletalMesh& sk = e->GetComponent<SkeletalMesh>(ent);
			if (sk.GetAnimControllerID() != Entity(-1, -1))
			{
				AnimationController& animCont = e->GetComponent<AnimationController>(sk.GetAnimControllerID());
				for (int i = 0; i < animCont.boneList->size(); i++)
				{
					glm::mat4 offSet = (*animCont.boneList)[i].offsetMat;
					Transform& boneT = e->GetComponent<Transform>(((*animCont.boneList)[i].eid));
					glm::mat4 tMat = boneT.transformMatrix();

					skRend->setUniformMat4fvArray("boneTransform", (*animCont.boneList)[i].id, glm::value_ptr(tMat));
					skRend->setUniformMat4fvArray("offsetMat", (*animCont.boneList)[i].id, glm::value_ptr(offSet));
				}
			}
			Transform& t = e->GetComponent<Transform>(ent);
			glm::mat4 tMat = t.transformMatrix();
			skRend->setUniformMat4fv("transform", 1, glm::value_ptr(tMat));
			skRend->setUniformVec3("viewPos", cam.transform.GetGlobalPosition());
			skRend->setUniformVec3("viewDir", cam.transform.forward());
			sk.renderMesh();
		}
	
}

void SkeletalMeshRenderingSystem::Run()
{
	std::shared_ptr<ECS> e = ecs.lock();
	if (SkeletalMesh::needsUpdate)
	{
		SkeletalMesh::needsUpdate = false;
		SkeletalMesh::setupMesh();
	}

	//creates a map of shader names->"list of entities that use this shader in their materials"
	if (needs_update)
	{
		needs_update = false;
		mShaderNameEntityMap.clear();
		for (auto ent : entities)
		{
			Material& m = e->GetComponent<Material>(ent);
			mShaderNameEntityMap[m.SHADER_NAME].push_back(ent);
		}
	}
	
}
