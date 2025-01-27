#include"Animation/SkinnedRendererPipeline.h"
#include"ECS.h"

#pragma warning(push, 0)
#include<glad/glad.h>
#pragma warning(pop)   

void SkeletalMeshRenderingPipeline::OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer)
{
	glEnable(GL_MULTISAMPLE);
	glm::mat4 camViewMat = cam.getViewMatrix();
	std::shared_ptr<LightSystem> lsys = rsys->lightsystem.lock();
	shader->setUniformInteger("num_of_dir_lights", static_cast<unsigned int>(lsys->drVector.size()));
	shader->setUniformMat4fv("viewMat", 1, glm::value_ptr(camViewMat));
	if (lsys->drVector.size() > 0)
	{
		
		shader->setUniformVec3("dir.lightDir", lsys->drVector[0].lightDir);
		shader->setUniformVec3("dir.lightColor", lsys->drVector[0].lightColor);
		shader->setUniformVec3("dir.ambient", lsys->drVector[0].ambient);
		shader->setUniformFloat("dir.intensity", lsys->drVector[0].intensity);
		shader->setUniformFloat("far", lsys->shadowDistance);
		shader->setUniformFloat("near", 0.1f);
		shader->setUniformFloat("lambda", lsys->lambda);
		shader->setUniformInteger("dir_sMap", 6);
		shader->setUniformInteger("numOfFrustum", static_cast<unsigned int>(lsys->dirLightSpace.size()));
		shader->setUniformFloat("shadowRes", static_cast<float>(lsys->dir_sMap.GetResolution()));
		
		lsys->dir_sMap.useTextureArray(6);
		for (int i = 0; i < lsys->dirLightSpace.size(); i++)
		{

			shader->setUniformMat4fvArray("lightSpaceMat", i, glm::value_ptr(lsys->dirLightSpace[i]));

		}


	}
	
}

void SkeletalMeshRenderingPipeline::OnEntityRender(std::shared_ptr<Shader> skRend, std::shared_ptr<ECS> e, Camera cam, Entity eid)
{
	SkeletalMesh& sk = e->GetComponent<SkeletalMesh>(eid);
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
	Transform& t = e->GetComponent<Transform>(eid);
	glm::mat4 tMat = t.transformMatrix();
	skRend->setUniformMat4fv("transform", 1, glm::value_ptr(tMat));
	skRend->setUniformVec3("viewPos", cam.transform.GetGlobalPosition());
	skRend->setUniformVec3("viewDir", cam.transform.forward());
}
