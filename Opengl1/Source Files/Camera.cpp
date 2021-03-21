#include "Camera.h"

Camera::Camera():Camera(60,aspectRatio,0.1f,100.0f,false)
{
}

Camera::Camera(GLfloat fovy, GLfloat aspectRatio, GLfloat nearz, GLfloat farz, GLboolean ortho)
{
	
	//cameraTarget = transform.position+glm::vec3(0, 0, -3);
	this->fovy = fovy;
	this->aspectRatio = aspectRatio;
	this->nearz = nearz;
	this->farz = farz;
	this->ortho = ortho;
	//updateCamDir();
}

void Camera::use()
{
	
}

glm::mat4 Camera::getViewMatrix()
{
	updateCamDir();
	static glm::vec3 up,right;
	right = glm::normalize(glm::cross(camDir,glm::vec3(0,1,0)));
	up = glm::normalize(glm::cross(right,camDir));
	return glm::lookAt(transform.position,cameraTarget,worldUp);

}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(fovy,aspectRatio,nearz,farz);
}

void Camera::setCameraPosition(GLfloat x, GLfloat y, GLfloat z)
{
	transform.position.x = x;
	transform.position.y = y;
	transform.position.z = z;
}

void Camera::setCameraPosition(glm::vec3 pos)
{
	transform.position = pos;
}

void Camera::setCameraRotation(glm::vec3 rot)
{
	transform.rotation.setEulerAngle(rot.x,rot.y,rot.z);
}

void Camera::setCameraTargetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	cameraTarget = glm::vec3(x,y,z) ;
}


void Camera::setCameraTargetPosition(glm::vec3 pos)
{
	cameraTarget = pos;
}
