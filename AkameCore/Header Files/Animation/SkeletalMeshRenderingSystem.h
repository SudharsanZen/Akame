#pragma once
#include"Core/Debug/Debug.h"
#include"Math/GlmMath.h"
#include<memory>
#include<set>
#include"ECS.h"
#include"Rendering/FrameBuffer.h"
#include"Rendering/System/ShadowFBO.h"
#include"Rendering/System/TiledRenderer.h"
#include<map>
#include<unordered_map>
#include"Components/Animation/SkeletalMesh.h"

class SkeletalMeshRenderingSystem: public System
{
	friend class Scene;
	std::weak_ptr<ECS> ecs;
public:
	void renderMeshes(Camera cam)
	{
		std::shared_ptr<ECS> e = ecs.lock();
		std::shared_ptr<Shader> skRend = ShaderManager::GetShader("SkinnedMeshRenderer");
		
		skRend->useShaderProgram();
		for (auto& ent : entities)
		{
			
			SkeletalMesh& sk = e->GetComponent<SkeletalMesh>(ent);
			Transform& t = e->GetComponent<Transform>(ent);
			glm::mat4 tMat = t.transformMatrix();
			skRend->setUniformMat4fv("transform", 1,glm::value_ptr(tMat) );
			skRend->setUniformVec3("viewPos",cam.transform.GetGlobalPosition());
			skRend->setUniformVec3("viewDir",cam.transform.forward());
			sk.renderMesh();
		}
	}
	void Run()
	{
		if (SkeletalMesh::needsUpdate)
		{
			SkeletalMesh::needsUpdate = false;
			SkeletalMesh::setupMesh();
		}
		/*
		//bone debug renderer
		std::shared_ptr<ECS> e = ecs.lock();
		for (auto& ent : entities)
		{
			SkeletalMesh& sk = e->GetComponent<SkeletalMesh>(ent);
			for (auto& bone : sk.boneMap)
			{
				Transform& curr = e->GetComponent<Transform>(bone.eid);
				glm::vec3 currPose = curr.GetGlobalPosition();
				Debug::DrawCircle(currPose, curr.up(), 0.1, glm::vec3(1, 0.5, 0));
				Debug::DrawCircle(currPose, curr.forward(), 0.1, glm::vec3(1, 0.5, 0));
				Debug::DrawCircle(currPose, curr.right(), 0.1, glm::vec3(1, 0.5, 0));
				Debug::DrawRay(curr.GetGlobalPosition(),curr.forward(),1,glm::vec3(0,0,1));
				Debug::DrawRay(curr.GetGlobalPosition(),curr.up(),1,glm::vec3(0,1,0));
				Debug::DrawRay(curr.GetGlobalPosition(),curr.right(),1,glm::vec3(1,0,0));
				if (bone.parentName != "")
				{
				
					
					Transform& parent = curr.getParentTransform();
					
					glm::vec3 parentPose = parent.GetGlobalPosition();
					Debug::DrawLine(currPose,parentPose,glm::vec3(0,1,0));

					
				
				}
				
			}
		}*/
	}
};

