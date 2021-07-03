#include "FrameBuffer.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<Log/Log.h>
#include"glm/glm/glm.hpp"
#include"Assets/ShaderManager.h"

vert Vtl = { glm::vec3(-1,1,0),glm::vec3(0,0,1),glm::vec2(0,1) };//top left vertex
vert Vtr = { glm::vec3(1,1,0),glm::vec3(0,0,1),glm::vec2(1,1) };//top right vertex
vert Vbl = { glm::vec3(-1,-1,0),glm::vec3(0,0,1),glm::vec2(0,0) };//bottom left
vert Vbr = { glm::vec3(1,-1,0),glm::vec3(0,0,1),glm::vec2(1,0) };//bottom right

std::vector<vert> quad = {Vtl,Vtr,Vbl,Vbr};
std::vector<unsigned int> indicess = {0,1,2,2,3,1};
void FrameBuffer::generateFrameBuffer()
{
	//generate quad mesh to render to
	quadMesh.CreateMesh(quad,indicess);
	//generate FrameBuffer object and bind it
	glGenFramebuffers(1,&framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);

	//Generate texture buffer
	glGenTextures(1,&texColorBuffer);
	glBindTexture(GL_TEXTURE_2D,texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,800,600,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	//attach the texture to the currently bound framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	//create render buffer
	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ENGINE_CORE_ERROR( "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
void FrameBuffer::updateTextureSize(int height,int width)
{

	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);


	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}
FrameBuffer::FrameBuffer()
{
	rbo = 0;
	texColorBuffer = 0;
	framebuffer = 0;
	generateFrameBuffer();
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	glClearColor(0.0f,0.1f,0.1f,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::unBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
}

void FrameBuffer::RenderToQuad()
{
	ShaderManager::GetShader("SCREENSHADER")->useShaderProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);

	
	quadMesh.renderMesh();
	glActiveTexture(0);
}

FrameBuffer::~FrameBuffer()
{
	if (texColorBuffer)
	{
		glDeleteTextures(1,&texColorBuffer);
		texColorBuffer = 0;
	}
	if (framebuffer)
		glDeleteFramebuffers(1, &framebuffer);
	if (rbo)
		glDeleteRenderbuffers(1, &rbo);
}
