#include"Core/Log/Log.h"
#include"Rendering/System/PSSMFBO.h"
#include"Rendering/System/LightSystem.h"

#pragma warning(push, 0)
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#pragma warning(pop)
void PSSMFBO::initialize()
{
	glGenFramebuffers(1,&fbo);
	glGenTextures(1,&shadowMapsArray);
	glBindTexture(GL_TEXTURE_2D_ARRAY,shadowMapsArray);
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_DEPTH_COMPONENT32F, resolution, resolution, numOfFrustum);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
	constexpr float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BORDER_COLOR, bordercolor);

	glBindFramebuffer(GL_FRAMEBUFFER,fbo);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadowMapsArray, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		ENGINE_CORE_ERROR("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

PSSMFBO::PSSMFBO(int numOfFrustum, int res)
{
	shadowMapsArray = 0;
	fbo = 0;
	resolution = res;
	this->numOfFrustum = numOfFrustum;
	initialize();
}

int PSSMFBO::GetResolution() 
{ 
	return resolution;
}

void PSSMFBO::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_TEXTURE_2D_ARRAY, shadowMapsArray, 0);

}

void PSSMFBO::clearBuffer()
{ 
	glClear(GL_DEPTH_BUFFER_BIT); 
}

void PSSMFBO::unbind(unsigned int frameBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
}

void PSSMFBO::useTextureArray(unsigned int unit) 
{ 
	glActiveTexture(GL_TEXTURE0 + unit); 
	glBindTexture(GL_TEXTURE_2D_ARRAY, shadowMapsArray); 
}

float CalSplitDistance(int farPlaneNum,int numOfFrustum, float znear, float zfar, float lambda)
{
	//float bias = 1.0f-((numOfFrustum-farPlaneNum)/(numOfFrustum))-(1.0f/numOfFrustum);
	//lambda += bias;
	float logrithimic = znear*pow(zfar/znear,(float)farPlaneNum/((float)numOfFrustum));
	float uniform = znear + (zfar - znear) * ((float)farPlaneNum /((float)numOfFrustum));
	float interPolatedValue = lambda*uniform+(1.0f-lambda)*logrithimic;
	return interPolatedValue;
}
std::vector<std::vector<glm::vec3>> CalculateFrustumCorners(Camera &cam,int numOfFrus,float lambda,float shadowDist)
{
	
	std::vector<std::vector<glm::vec3>> frustumList;
	glm::vec3 camUp = cam.transform.up();
	glm::vec3 camRight = cam.transform.right();

	float nearz = cam.getNear(), farz =shadowDist;

	glm::vec3 viewDir = cam.transform.forward();

	glm::vec3 nearPoint = cam.transform.GetGlobalPosition() + viewDir * nearz;
	glm::vec3 farPoint = cam.transform.GetGlobalPosition()+viewDir * farz;
	glm::vec3 camPose = cam.transform.GetGlobalPosition();
	float mHeight = 2*tan(cam.getFov()/2)*farz;
	float mWidth = cam.getAspectRatio()*mHeight;

	glm::vec3 nextNearP = nearPoint;
	float nextNearZ=nearz;

	for (int i = 1; i <= numOfFrus; i++)
	{ 
		float nH = nextNearZ*mHeight/farz;
		float nW = nextNearZ*mWidth/farz;

		std::vector<glm::vec3> frustumCorners;
		float ifar = CalSplitDistance(i,numOfFrus,nearz,farz,lambda);
		float ifW = ifar * mWidth / farz;
		float ifH = ifar * mHeight / farz;

		glm::vec3 nearp=nextNearP, farp=camPose+viewDir*ifar;
		
		frustumCorners.push_back(nearp+camRight*nW/2.0f+camUp*nH/2.0f);
		frustumCorners.push_back(nearp-camRight*nW/2.0f-camUp*nH/2.0f);
		frustumCorners.push_back(nearp+camRight*nW/2.0f-camUp*nH/2.0f);
		frustumCorners.push_back(nearp-camRight*nW/2.0f+camUp*nH/2.0f);

		frustumCorners.push_back(farp + camRight * ifW / 2.0f + camUp * ifH / 2.0f);
		frustumCorners.push_back(farp - camRight * ifW / 2.0f - camUp * ifH / 2.0f);
		frustumCorners.push_back(farp + camRight * ifW / 2.0f - camUp * ifH / 2.0f);
		frustumCorners.push_back(farp - camRight * ifW / 2.0f + camUp * ifH / 2.0f);
		nextNearP = farp;
		nextNearZ = ifar;
		frustumList.push_back(frustumCorners);
	}

	return frustumList;
}
std::vector<glm::mat4> CalculatePSSMLightSpaceMats(Camera& cam, glm::vec3  l, int numOfFrustums,float lambda,float shadowDist)
{

	std::vector<glm::mat4> matList;
	glm::vec3 lightDir = l;
	glm::vec3 viewDir = cam.transform.forward();
	glm::mat4 cp,cv;

	glm::vec3 forward = glm::normalize(lightDir);
	glm::vec3 pose =glm::vec3(0);
	glm::vec3 up = normalize(glm::cross(forward, -worldRight));
	glm::vec3 right = normalize(glm::cross(forward, up));
	glm::vec3 center = pose + forward * 10.0f;


	glm::mat4 lightViewMat=glm::lookAt(pose,center,up);
	/*glm::mat4 lightViewMat = glm::identity<glm::mat4>();
	lightViewMat[0] = glm::vec4(-right,0);
	lightViewMat[1] = glm::vec4(up,0);
	lightViewMat[2] = glm::vec4(forward,0);
	*/
	
	float boxSize = 50;

	glm::mat4 projOrtho = glm::ortho(-boxSize, boxSize, -boxSize, boxSize, 0.1f, 100.0f);
	
	float nearz = cam.getNear();
	float farz = cam.getFar();
	cp = cam.getProjectionMatrix();
	cv = cam.getViewMatrix();
	std::vector<std::vector<glm::vec3>> frustumList=CalculateFrustumCorners(cam,numOfFrustums,lambda,shadowDist);
	for (int i = 0; i < frustumList.size(); i++)
	{
	
		std::vector<glm::vec3> currFrustum = frustumList[i];
		glm::vec3 max = lightViewMat * glm::vec4(currFrustum[0], 1);
		glm::vec3 min=max;
		for (int i_vert = 0; i_vert < currFrustum.size(); i_vert++)
		{
			

			glm::vec3 curr = lightViewMat*glm::vec4(currFrustum[i_vert],1);

			if (max.x < curr.x)
				max.x = curr.x;
			if (max.y < curr.y)
				max.y = curr.y;
			if (max.z < curr.z)
				max.z = curr.z;
			
			if (min.x > curr.x)
				min.x = curr.x;
			if (min.y > curr.y)
				min.y = curr.y;
			if (min.z > curr.z)
				min.z = curr.z;
	
		}

		glm::vec3 center = ((max+min)/2.0f);
		max = max - center;
		min = min - center;

		
		float nearz=cam.getNear(), farz=shadowDist;
		glm::vec3 viewDir = cam.transform.forward();
		glm::vec3 camPose = cam.transform.GetGlobalPosition();
		float iN = CalSplitDistance(i,numOfFrustums,nearz,farz,lambda);
		float iF = CalSplitDistance(i+1,numOfFrustums,nearz,farz,lambda);
		glm::vec3 cen = camPose+viewDir*((iN+iF)/2.0f);
		glm::vec3 pose = cen-forward*200.0f;
		glm::mat4 lViewMat=glm::lookAt(pose,cen+forward,up);
		float x=1, y=1;
		if (i < 2)
		{
			x *= LightSystem::pssmXMultiplier;
			y *= LightSystem::pssmYMultiplier;
		}
		glm::mat4 lProjMat = glm::ortho(min.x*x,max.x*x,min.y*y,max.y*y,-100.0f,1000.0f);

		matList.push_back(lProjMat*lViewMat);
	}


	return matList;
}

std::vector<AABB> Calculate_PSSM_AABB_Bounds(Camera& cam, glm::vec3  l, int numOfFrustums, float lambda, float shadowDist)
{

	std::vector<AABB> aabb_list;
	glm::vec3 lightDir = l;
	glm::vec3 viewDir = cam.transform.forward();
	glm::mat4 cp, cv;

	glm::vec3 forward = glm::normalize(lightDir);
	glm::vec3 pose = glm::vec3(0);
	glm::vec3 up = normalize(glm::cross(forward, -worldRight));
	glm::vec3 right = normalize(glm::cross(forward, up));
	glm::vec3 center = pose + forward * 10.0f;




	float nearz = cam.getNear();
	float farz = cam.getFar();
	cp = cam.getProjectionMatrix();
	cv = cam.getViewMatrix();
	std::vector<std::vector<glm::vec3>> frustumList = CalculateFrustumCorners(cam, numOfFrustums, lambda, shadowDist);
	for (int i = 0; i < frustumList.size(); i++)
	{

		std::vector<glm::vec3> currFrustum = frustumList[i];
		glm::vec3 max = glm::vec4(currFrustum[0], 1);
		glm::vec3 min = max;
		for (int i_vert = 0; i_vert < currFrustum.size(); i_vert++)
		{


			glm::vec3 curr = glm::vec4(currFrustum[i_vert], 1);

			if (max.x < curr.x)
				max.x = curr.x;
			if (max.y < curr.y)
				max.y = curr.y;
			if (max.z < curr.z)
				max.z = curr.z;

			if (min.x > curr.x)
				min.x = curr.x;
			if (min.y > curr.y)
				min.y = curr.y;
			if (min.z > curr.z)
				min.z = curr.z;

		}

		float x = 1, y = 1;
		if (i < 2)
		{
			x *= LightSystem::pssmXMultiplier;
			y *= LightSystem::pssmYMultiplier;
		}

		glm::vec3 minb = min; minb.x *= x; minb.y *= y;
		glm::vec3 maxb = max; maxb.x *= x; maxb.y *= y;
		AABB bound(minb, maxb);

		aabb_list.push_back(bound);
	}


	return aabb_list;
}