#include"glad/glad.h"
#include"GLFW/glfw3.h"
#include"Systems/RenderingSystem/ShadowFBO.h"
#include"Log/Log.h"
#include<iostream>
void ShadowFBO::initFBO(int height, int width)
{

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);

	//texture for depth data
	glGenTextures(1, &outDepth);
	glBindTexture(GL_TEXTURE_2D, outDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,width,height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, outDepth, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ENGINE_CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowFBO::bindShadowBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glClearColor(1,1,1, 1);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowFBO::unBindShadowBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void ShadowFBO::useDepthTexture(unsigned int textureUnit)
{
	glActiveTexture(GL_TEXTURE0+textureUnit);
	glBindTexture(GL_TEXTURE_2D,outDepth);
}

ShadowFBO::~ShadowFBO()
{
	if (frameBuffer)
	{
		glDeleteFramebuffers(1,&frameBuffer);
		glDeleteTextures(1,&outDepth);
		frameBuffer = 0;
		outDepth = 0;
	}
}
