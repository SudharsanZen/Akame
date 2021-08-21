#include "Systems/RenderingSystem/LightSystem.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
void LightSystem::updatePointLightBuffer()
{
	ptVector.clear();
	std::shared_ptr<ECS> e = ecs.lock();
	pointLight pl;
	for (auto& ent : lightsList[LIGHT::POINT])
	{
		
		Lights& l = e->GetComponent<Lights>(ent);
		pl.lightPose = glm::vec4(e->GetComponent<Transform>(ent).position,1);
		pl.ambient = glm::vec4(l.ambient,1);
		pl.constants = glm::vec4(l.pointLightConstants, l.getPointLightRadius());
		pl.lightColor = glm::vec4(l.lightColor, 1);
		pl.intensity = l.intensity;
		
		ptVector.push_back(pl);
	}
	

	
	int offs = 0;
	glBindBuffer(GL_UNIFORM_BUFFER, plUBO);
	for (size_t i = 0; i < ptVector.size(); i++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offs, 4 * sizeof(float), &ptVector[i].lightPose);
		offs += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offs, 4 * sizeof(float), &ptVector[i].lightColor);
		offs += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offs, 4 * sizeof(float), &ptVector[i].ambient);
		offs += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offs, 4 * sizeof(float), &ptVector[i].constants);
		offs += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offs, 1 * sizeof(float), &ptVector[i].intensity);
		offs += 16;
	
	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	drVector.clear();

	for (auto& ent : lightsList[LIGHT::DIRECTIONAL])
	{
		Lights& l = ecs.lock()->GetComponent<Lights>(ent);
		directionalLight drl;
		drl.lightDir = l.lightDirection;
		drl.lightColor = l.lightColor;
		drl.ambient = l.ambient;
		drl.intensity = l.intensity;
		drVector.push_back(drl);
	}
}
void LightSystem::updatePointLightContents()
{
	std::shared_ptr<ECS> e = ecs.lock();
	pointLight pl;
	for (int i=0; i<ptVector.size();i++)
	{
		
		Lights& l = e->GetComponent<Lights>(lightsList[LIGHT::POINT][i]);
		pl.lightPose = glm::vec4(e->GetComponent<Transform>(lightsList[LIGHT::POINT][i]).position,1);
		pl.ambient = glm::vec4(l.ambient, 1);
		pl.constants = glm::vec4(l.pointLightConstants, l.getPointLightRadius());
		pl.lightColor = glm::vec4(l.lightColor, 1);
		pl.intensity = l.intensity;
	
		ptVector[i]=pl;
	}

	
	int offs = 0;
	glBindBuffer(GL_UNIFORM_BUFFER, plUBO);
	for (size_t i = 0; i < ptVector.size(); i++)
	{
		glBufferSubData(GL_UNIFORM_BUFFER, offs, 4 * sizeof(float), &ptVector[i].lightPose);
		offs += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offs, 4 * sizeof(float), &ptVector[i].lightColor);
		offs += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offs, 4 * sizeof(float), &ptVector[i].ambient);
		offs += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offs, 4 * sizeof(float), &ptVector[i].constants);
		offs += 16;
		glBufferSubData(GL_UNIFORM_BUFFER, offs, 1 * sizeof(float), &ptVector[i].intensity);
		offs += 16;

	}
	glBindBuffer(GL_UNIFORM_BUFFER, 0);


	for (int i=0;i<drVector.size();i++)
	{
		Lights& l = e->GetComponent<Lights>(lightsList[LIGHT::DIRECTIONAL][i]);
		directionalLight &drl=drVector[i];
		drl.lightDir = l.lightDirection;
		drl.lightColor = l.lightColor;
		drl.ambient = l.ambient;
		drl.intensity = l.intensity;
	
	}

}
void LightSystem::bindPointLightBuffer(int layoutIndex)
{
	glBindBufferBase(GL_UNIFORM_BUFFER,layoutIndex,plUBO);
	
}
LightSystem::LightSystem() :dir_sMap(DIR_MAP_SIZE, DIR_MAP_SIZE)
{
	glGenBuffers(1, &plUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, plUBO);
	glBufferData(GL_UNIFORM_BUFFER,800*80,NULL,GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);
	

}

void LightSystem::BindDirectionalLightShadowMap(std::shared_ptr<Shader> shader)
{

	//currently only rendering one shadow map
	if (drVector.size() > 0)
	{
		glCullFace(GL_FRONT);
		glViewport(0, 0, DIR_MAP_SIZE, DIR_MAP_SIZE);
		auto const& drLight = drVector[0];

		glm::vec3 forward = glm::normalize(drLight.lightDir);
		glm::vec3 pose = -forward * 36.0f;
		glm::vec3 up = normalize(glm::cross(forward, -worldRight));

		
		dir_sMap.bindShadowBuffer();
		shader->useShaderProgram();
		glm::mat4 viewMat = glm::lookAt(pose, pose + (forward * 10.0f), up);

		float boxSize = 50;

		glm::mat4 projOrtho = glm::ortho(-boxSize, boxSize, -boxSize, boxSize, 0.1f, 100.0f);
		shader->setUniformMat4fv("projMat", 1, glm::value_ptr(projOrtho));
		shader->setUniformMat4fv("view", 1, glm::value_ptr(viewMat));
		dirLightSpace = projOrtho * viewMat;





	}
}
void LightSystem::unBindDirectionalShadowMap()
{

	dir_sMap.unBindShadowBuffer();
	glCullFace(GL_BACK);
}