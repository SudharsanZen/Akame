#include "DeferredRendererFragmentBuffer.h"
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"Log/Log.h"


vert vtl = { glm::vec3(-1,1,0),glm::vec3(0,0,1),glm::vec2(0,1) };//top left vertex
vert vtr = { glm::vec3(1,1,0),glm::vec3(0,0,1),glm::vec2(1,1) };//top right vertex
vert vbl = { glm::vec3(-1,-1,0),glm::vec3(0,0,1),glm::vec2(0,0) };//bottom left
vert vbr = { glm::vec3(1,-1,0),glm::vec3(0,0,1),glm::vec2(1,0) };//bottom right

std::vector<vert> quadVert = { vtl,vtr,vbl,vbr };
std::vector<unsigned int> indices = { 0,1,2,2,3,1 };
DeferredRendererFragmentBuffer::DeferredRendererFragmentBuffer()
{
	quad.CreateMesh(quadVert,indices);
	GenerateFrameBuffer();
}

void DeferredRendererFragmentBuffer::GenerateFrameBuffer()
{
	
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);


	//texture for storing albedo and specular
	//rgb for albedo and alpha for specular 
	glGenTextures(1, &AlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, AlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, AlbedoSpec, 0);

	//createting texture with 16 bit bit float components for storing more precise positional and normal values
	//texture for positional data
	glGenTextures(1, &Position);
	glBindTexture(GL_TEXTURE_2D, Position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, Position, 0);

	//texture for normal data
	glGenTextures(1, &Normal);
	glBindTexture(GL_TEXTURE_2D, Normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D,Normal, 0);

	unsigned int frameBufferAttachments[]= {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2 };

	glDrawBuffers(3,frameBufferAttachments);

	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ENGINE_CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DeferredRendererFragmentBuffer::updateBufferSize(int height,int width)
{
	glBindTexture(GL_TEXTURE_2D, AlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	glBindTexture(GL_TEXTURE_2D, Position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	
	glBindTexture(GL_TEXTURE_2D, Normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
	
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void DeferredRendererFragmentBuffer::bindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);
	glClearColor(0.0f, 0.0f, 0.f, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DeferredRendererFragmentBuffer::unBindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}
//tex unit 1: albedoSpec,unit 2:Normal,unit 3:position
void DeferredRendererFragmentBuffer::useTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AlbedoSpec);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, Position);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, Normal);

}

void DeferredRendererFragmentBuffer::outPutToQaud(Camera &cam,std::shared_ptr<Shader> shader)
{
	
	//deferred renderer final pass shader
	useTextures();
	shader->setUniformInteger("AlbedoSpec",0);
	shader->setUniformInteger("Position",1);
	shader->setUniformInteger("Normal",2);
	shader->setUniformVec3("viewPos",cam.transform.position);

	quad.renderMesh();
}

void DeferredRendererFragmentBuffer::drawPositionBuffer()
{
	std::shared_ptr<Shader> shader = ShaderManager::GetShader("SCREENSHADER");
	shader->useShaderProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Position);
	quad.renderMesh();

}

void DeferredRendererFragmentBuffer::drawNormalBuffer()
{
	std::shared_ptr<Shader> shader = ShaderManager::GetShader("SCREENSHADER");
	shader->useShaderProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Normal);
	quad.renderMesh();
}

void DeferredRendererFragmentBuffer::drawAlbedoBuffer()
{
	std::shared_ptr<Shader> shader = ShaderManager::GetShader("SCREENSHADER");
	shader->useShaderProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AlbedoSpec);
	quad.renderMesh();
}

DeferredRendererFragmentBuffer::~DeferredRendererFragmentBuffer()
{
	if (Normal)
		glDeleteTextures(1,&Normal);
	if (AlbedoSpec)
		glDeleteTextures(1, &AlbedoSpec);
	if (Position)
		glDeleteTextures(1, &Position);
	if (frameBuffer)
		glDeleteFramebuffers(1,&frameBuffer);
	if (rbo)
		glDeleteRenderbuffers(1,&rbo);
	Normal = 0;
	AlbedoSpec = 0;
	Position = 0;
	rbo = 0;
	frameBuffer = 0;
	
}
