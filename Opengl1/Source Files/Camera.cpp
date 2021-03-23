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
	/*updateCamDir();
	static glm::vec3 up,right;
	right = glm::normalize(glm::cross(camDir,glm::vec3(0,1,0)));
	up = glm::normalize(glm::cross(right,camDir));*/
	return glm::lookAt(transform.position,transform.position+(transform.forward()*10.0f),transform.up());
	//return lookAt(transform,transform.position);

}

glm::mat4 Camera::getProjectionMatrix()
{
	return glm::perspective(fovy,aspectRatio,nearz,farz);
}

glm::mat4 Camera::lookAt(Transform& transform, glm::vec3& pos)
{
	glm::vec3 const f(transform.forward());
	glm::vec3 const s(transform.right());
	glm::vec3 const u(transform.up());

	glm::mat4 Result(1);
	Result[0][0] = s.x;
	Result[1][0] = s.y;
	Result[2][0] = s.z;
	Result[0][1] = u.x;
	Result[1][1] = u.y;
	Result[2][1] = u.z;
	Result[0][2] = -f.x;
	Result[1][2] = -f.y;
	Result[2][2] = -f.z;
	Result[3][0] = -glm::dot(s, pos);
	Result[3][1] = -glm::dot(u, pos);
	Result[3][2] = glm::dot(f, pos);
	return Result;
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

/*void Camera::setCameraTargetPosition(GLfloat x, GLfloat y, GLfloat z)
{
	cameraTarget = glm::vec3(x,y,z) ;
}


void Camera::setCameraTargetPosition(glm::vec3 pos)
{
	cameraTarget = pos;
}*/
