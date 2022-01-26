#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include "Rendering\System\DefaultRenderingPipeline.h"
#include<sstream>
void DefaultRenderingPipeline::WindowsResizeCallBacks(int height, int width)
{
}

void DefaultRenderingPipeline::OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer)
{
	glEnable(GL_MULTISAMPLE);
	std::shared_ptr<LightSystem> lsys = rsys->lightsystem.lock();
	glm::mat4 camViewMat = cam.getViewMatrix();
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
		shader->setUniformInteger("dir_sMap",6);
		shader->setUniformInteger("numOfFrustum",lsys->dirLightSpace.size());
		shader->setUniformFloat("shadowRes",lsys->dir_sMap.GetResolution());
		
		lsys->dir_sMap.useTextureArray(6);
		for (int i = 0; i < lsys->dirLightSpace.size(); i++)
		{
			
			shader->setUniformMat4fvArray("lightSpaceMat", i, glm::value_ptr(lsys->dirLightSpace[i]));

		}
			
		
	}
}

void DefaultRenderingPipeline::OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer)
{
}
