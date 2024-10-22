#include "Rendering/DeferredRendererFragmentBuffer.h"
#include"Core/Log/Log.h"
#pragma warning(push, 0)
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#pragma warning(pop)


vert vtl = { glm::vec3(-1,1,0),glm::vec3(0,0,1),glm::vec2(0,1) };//top left vertex
vert vtr = { glm::vec3(1,1,0),glm::vec3(0,0,1),glm::vec2(1,1) };//top right vertex
vert vbl = { glm::vec3(-1,-1,0),glm::vec3(0,0,1),glm::vec2(0,0) };//bottom left
vert vbr = { glm::vec3(1,-1,0),glm::vec3(0,0,1),glm::vec2(1,0) };//bottom right

	std::vector<vert> BasicShapes::quadVert = { vtl,vtr,vbl,vbr };
	std::vector<unsigned int> BasicShapes::quadIndices = { 2,1,0,2,3,1 };

DeferredRendererFragmentBuffer::DeferredRendererFragmentBuffer()
{
	quad.CreateMesh(BasicShapes::quadVert,BasicShapes::quadIndices);
	GenerateFrameBuffer();
}

void DeferredRendererFragmentBuffer::GenerateFrameBuffer()
{
	
	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);


	//texture for storing albedo and specular
	//rgb for albedo and alpha for specular 
	glGenTextures(1, &AlbedoRough);
	glBindTexture(GL_TEXTURE_2D, AlbedoRough);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, AlbedoRough, 0);

	//createting texture with 16 bit bit float components for storing more precise positional and normal values
	//texture for positional data
	glGenTextures(1, &Position);
	glBindTexture(GL_TEXTURE_2D, Position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);
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

	//PBR buffer
	glGenTextures(1, &PBR);
	glBindTexture(GL_TEXTURE_2D, PBR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 600, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, PBR, 0);

	//texture for depth data
	glGenTextures(1, &depthBuffer);
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 600, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);

	unsigned int frameBufferAttachments[]= {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };

	glDrawBuffers(4,frameBufferAttachments);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ENGINE_CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int DeferredRendererFragmentBuffer::getAlbedoBuff() { return AlbedoRough; }

unsigned int DeferredRendererFragmentBuffer::getDepthBuff() { return depthBuffer; }

unsigned int DeferredRendererFragmentBuffer::getNormalBuff() { return this->Normal; }

unsigned int DeferredRendererFragmentBuffer::getPositionBuff() { return Position; }

void DeferredRendererFragmentBuffer::updateBufferSize(int height,int width)
{
	glBindTexture(GL_TEXTURE_2D, AlbedoRough);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	glBindTexture(GL_TEXTURE_2D, Position);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	
	glBindTexture(GL_TEXTURE_2D, Normal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

	glBindTexture(GL_TEXTURE_2D, PBR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	glBindTexture(GL_TEXTURE_2D, depthBuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	

}

void DeferredRendererFragmentBuffer::bindFrameBuffer()
{
	glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);
	glClearColor(0.0f, 0.0f, 0.f, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void DeferredRendererFragmentBuffer::unBindFrameBuffer(unsigned int frameBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER,frameBuffer);
}
//tex unit 1: albedoSpec,unit 2:Normal,unit 3:position
void DeferredRendererFragmentBuffer::useTextures()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, AlbedoRough);
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
	shader->setUniformVec3("viewPos",cam.transform.GetGlobalPosition());

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
	glBindTexture(GL_TEXTURE_2D, AlbedoRough);
	quad.renderMesh();
}

DeferredRendererFragmentBuffer::~DeferredRendererFragmentBuffer()
{
	if (Normal)
		glDeleteTextures(1,&Normal);
	if (AlbedoRough)
		glDeleteTextures(1, &AlbedoRough);
	if (PBR)
		glDeleteTextures(1, &PBR);
	if (Position)
		glDeleteTextures(1, &Position);
	if (frameBuffer)
		glDeleteFramebuffers(1,&frameBuffer);
	if (depthBuffer)
		glDeleteTextures(1,&depthBuffer);
	Normal = 0;
	AlbedoRough = 0;
	Position = 0;
	depthBuffer = 0;
	frameBuffer = 0;
	PBR = 0;
	
}
