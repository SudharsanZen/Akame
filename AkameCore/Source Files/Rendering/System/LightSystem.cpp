#include "Rendering/System/LightSystem.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<sstream>

float LightSystem::pssmXMultiplier = 1.45f;
float LightSystem::pssmYMultiplier = 1.45f;
bool LightSystem::needsUpdate=true;
void LightSystem::updateLightList()
{
	lightsList.clear();
	for (auto const& ent : entities)
	{
		Lights& l = ecs.lock()->GetComponent<Lights>(ent);
		lightsList[l.getType()].push_back(ent);
	}
}
void LightSystem::updatePointLightBuffer()
{
	ptVector.clear();
	std::shared_ptr<ECS> e = ecs.lock();
	pointLight pl;
	for (auto& ent : lightsList[LIGHT::POINT])
	{
		
		Lights& l = e->GetComponent<Lights>(ent);
		pl.lightPose = glm::vec4(e->GetComponent<Transform>(ent).GetGlobalPosition(),1);
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
		drl.lightDir = l.getDirection();
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
		pl.lightPose = glm::vec4(e->GetComponent<Transform>(lightsList[LIGHT::POINT][i]).GetGlobalPosition(),1);
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
		drl.lightDir = l.getDirection();
		drl.lightColor = l.lightColor;
		drl.ambient = l.ambient;
		drl.intensity = l.intensity;
	
	}

}
void LightSystem::bindPointLightBuffer(int layoutIndex)
{
	
	glBindBufferBase(GL_UNIFORM_BUFFER,layoutIndex,plUBO);
	
}
LightSystem::LightSystem() :dir_sMap(FRUSTUM_SPLIT_NUM,DIR_MAP_SIZE)
{
	glGenBuffers(1, &plUBO);
	glBindBuffer(GL_UNIFORM_BUFFER, plUBO);
	glBufferData(GL_UNIFORM_BUFFER,800*80,NULL,GL_DYNAMIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER,0);
	dirLightSpace = std::vector<glm::mat4>(FRUSTUM_SPLIT_NUM);

}

AKAME_API void LightSystem::Update()
{
	if (needsUpdate)
	{
		updateLightList();
		updatePointLightBuffer();
		needsUpdate = false;
	}
}

void LightSystem::OnAddEntity(Entity entity)
{

	updateLightList();
	updatePointLightBuffer();



}

void LightSystem::AfterDestroyEntity()
{
	updateLightList();
	updatePointLightBuffer();
}

void LightSystem::BindDirectionalLightShadowMap(std::shared_ptr<Shader> shader,Camera &cam)
{

	//currently only rendering one shadow map
	if (drVector.size() > 0)
	{
		drVector.clear();
		for (auto& ent : lightsList[LIGHT::DIRECTIONAL])
		{

			Lights& l = ecs.lock()->GetComponent<Lights>(ent);
			directionalLight drl;
			drl.lightDir = l.getDirection();
			drl.lightColor = l.lightColor;
			drl.ambient = l.ambient;
			drl.intensity = l.intensity;
			drVector.push_back(drl);
		}
		glCullFace(GL_FRONT);
		glViewport(0, 0, DIR_MAP_SIZE, DIR_MAP_SIZE);
		auto const& drLight = drVector[0];
		glm::vec3 forward = glm::normalize(drLight.lightDir);

		dir_sMap.bind();
		shader->useShaderProgram();
		shader->setUniformInteger("numOfFrustum", FRUSTUM_SPLIT_NUM);
		std::vector<glm::mat4> lightSpaceMatList = CalculatePSSMLightSpaceMats(cam,forward,FRUSTUM_SPLIT_NUM,lambda,shadowDistance);
		
		for (int i = 0; i < FRUSTUM_SPLIT_NUM; i++)
		{
			shader->setUniformMat4fvArray("lightSpaceMat", i, glm::value_ptr(lightSpaceMatList[i]));
			dirLightSpace[i] = lightSpaceMatList[i];
		}

		
		
	}

}

//get the directional light's view projection matrix

std::vector<glm::mat4> LightSystem::GetDirectionalLightSpaceMat()
{
	return dirLightSpace;
}
void LightSystem::unBindDirectionalShadowMap(unsigned int frameBuffer)
{

	dir_sMap.unbind(frameBuffer);
	glCullFace(GL_BACK);
	
}