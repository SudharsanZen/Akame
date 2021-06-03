#pragma once
#include"Engine.h"

class RenderingSystem:public System
{
private:
	glm::vec3 lightPose = glm::vec3(10, 4,10);
public:

	void Run(std::shared_ptr<ECS> ecs,Camera &cam)
	{
		
		for (auto const &ent : entities)
		{
			Transform& t =ecs->GetComponent<Transform>(ent) ;
			Mesh& mesh =ecs->GetComponent<Mesh>(ent);
			ecs->GetComponent<Material>(ent).use(t,lightPose,cam.transform.position);
			mesh.renderMesh();


		}
	}

};

