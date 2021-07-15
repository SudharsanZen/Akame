#include "Systems/LightSystem.h"
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
		pl.constants = glm::vec4(l.pointLightConstants, 1);
		pl.lightColor = glm::vec4(l.lightColor, 1);
		pl.intensity = l.intensity;
	
		ptVector.push_back(pl);
	}
	

	
	int offs = 0;
	glBindBuffer(GL_UNIFORM_BUFFER, plUBO);
	for (long long int i = 0; i < ptVector.size(); i++)
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
		pl.constants = glm::vec4(l.pointLightConstants, 1);
		pl.lightColor = glm::vec4(l.lightColor, 1);
		pl.intensity = l.intensity;

		ptVector[i]=pl;
	}

	
	int offs = 0;
	glBindBuffer(GL_UNIFORM_BUFFER, plUBO);
	for (long long int i = 0; i < ptVector.size(); i++)
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

}
void LightSystem::bindPointLightBuffer(int layoutIndex)
{
	glBindBufferBase(GL_UNIFORM_BUFFER,layoutIndex,plUBO);
	
}
LightSystem::LightSystem()
{
	glGenBuffers(1, &plUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, plUBO);
	glBufferData(GL_UNIFORM_BUFFER,800*80,NULL,GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);
	

}
