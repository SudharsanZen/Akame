#include"System.h"
#include"Mesh.h"
#include"Material.h"
#include "ECS.h"
#include "Systems/RenderingSystem.h"
#include"Camera.h"
void RenderingSystem::Run(std::shared_ptr<ECS> ecs, Camera& cam)
{

	for (auto const& ent : entities)
	{
		
		Transform& t = ecs->GetComponent<Transform>(ent);
		Mesh& mesh = ecs->GetComponent<Mesh>(ent);
		ecs->GetComponent<Material>(ent).use(t, lightPose, cam.transform.position);
		mesh.renderMesh();


	}
}