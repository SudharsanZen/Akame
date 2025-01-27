#include<sstream>
#include "Rendering/System/DeferredPipeline.h"
#pragma warning(push, 0)
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#pragma warning(pop)

DeferredPipeline::DeferredPipeline(int height, int width) 
{ 
	drfb.updateBufferSize(height, width); 
}
void DeferredPipeline::WindowsResizeCallBacks(int height, int width)
{
	drfb.updateBufferSize(height, width);
}
void DeferredPipeline::OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer)
{
	glDisable(GL_BLEND);
	//deferred renderer-----------------------------------------------------------------------
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_MULTISAMPLE);

	drfb.bindFrameBuffer();
}

 void DeferredPipeline::OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer)
{

	std::shared_ptr<LightSystem> lsys = rsys->lightsystem.lock();
	int height = rsys->height, width = rsys->width;

	drfb.unBindFrameBuffer(frameBuffer);


	//render final to quad
	glDisable(GL_DEPTH_TEST);//disable depth to remove quad from depth calulations

	drfb.setUpShader(cam, lsys);
	lsys->dir_sMap.useTextureArray(6);
	drfb.set4x4Matrixfv("proj", cam.getProjectionMatrix());
	drfb.set4x4Matrixfv("viewMat", cam.getViewMatrix());
	drfb.setFloat("lambda",lsys->lambda);
	drfb.setFloat("near",cam.getNear());
	drfb.setFloat("far",lsys->shadowDistance);
	drfb.setInt("shadowRes",(int)lsys->dir_sMap.GetResolution());
	drfb.setVec3("viewDirection",cam.transform.forward());
	lsys->dir_sMap.useTextureArray(6);
	for (int i = 0; i < lsys->dirLightSpace.size(); i++)
	{
		drfb.set4x4MatrixfvArray("lightSpaceMat[0]",i,lsys->dirLightSpace[i]);
		
	}
	
	drfb.outPutToQaud();


	//copy deferred rendering depth buffer to forward
	glBindFramebuffer(GL_READ_FRAMEBUFFER, drfb.drfb.frameBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frameBuffer); // write to default framebuffer
	glBlitFramebuffer(
		0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST
	);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

 void RM_SKY_BOXPipeline::OnPreRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer)
 {
	 std::shared_ptr<LightSystem> lsys = rsys->lightsystem.lock();
	 shader->useShaderProgram();

	 if(lsys->drVector.size()>0)
	 shader->setUniformVec3("sunPose", -lsys->drVector[0].lightDir);
 }

 void RM_SKY_BOXPipeline::OnPostRender(std::shared_ptr<Shader> shader, RenderingSystem* rsys, Camera cam, unsigned int frameBuffer)
 {

 }
