
#include "Rendering/DepthFrameBuffer.h"
#pragma warning(push, 0)
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#pragma warning(pop)
void DepthFrameBuffer::generateFrameBuffer()
{
	
	glGenFramebuffers(1,&framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

	glGenTextures(1,&depthTex);
	glBindTexture(GL_TEXTURE_2D,depthTex);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,map_width,map_height,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

DepthFrameBuffer::DepthFrameBuffer(int width, int  height)
{
	map_height = height;
	map_width = width;
	framebuffer = 0;;
	depthTex = 0;
	generateFrameBuffer();
}

void DepthFrameBuffer::updateTextureSize(int height, int width)
{
	glBindTexture(GL_TEXTURE_2D,depthTex);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,map_width,map_height,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void DepthFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	glClearColor(0,0,0,1);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void DepthFrameBuffer::unBind()
{
}

void DepthFrameBuffer::RenderToQuad()
{
}

DepthFrameBuffer::~DepthFrameBuffer()
{
}
