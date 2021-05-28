#include "Camera.h"
#include"Input.h"
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


//Resturns a 4x4 view matrix of type glm::mat4.
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



//A default implementation of a flycam.
void flyCam(Camera& cam,float deltaTime)
{

	static bool init = false;
	static double xPrev = 0, yPrev = 0;
	double x = 0, y = 0;
	static float X = -45, Y = -135;
	float speed = 10;
	if (Input::getKeyDown(GLFW_KEY_LEFT_SHIFT))
	{
		speed *= 5;
	}
	if (Input::getKeyDown(GLFW_KEY_W))
	{
		cam.setCameraPosition(cam.getCameraPosition() + cam.transform.forward() * speed * deltaTime);

	}
	if (Input::getKeyDown(GLFW_KEY_S))
	{
		cam.setCameraPosition(cam.getCameraPosition() - cam.transform.forward() * speed * deltaTime);

	}
	if (Input::getKeyDown(GLFW_KEY_A))
	{
		cam.setCameraPosition(cam.getCameraPosition() - cam.transform.right() * speed * deltaTime);

	}
	if (Input::getKeyDown(GLFW_KEY_D))
	{
		cam.setCameraPosition(cam.getCameraPosition() + cam.transform.right() * speed * deltaTime);

	}
	if (init)
		Input::getMouseXY(x, y);

	if (Input::getMouseButton(GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS || !init)
	{
		init = true;
		X += -(y - yPrev) * 0.5;
		Y += -(x - xPrev) * 0.5;
		//if (abs(Y) >= 360.0f)
			//Y =Y+((Y>0)?-1:1)*360.0f;
		Y = abs(Y) >= 360.0f ? 0 : Y;
		X = abs(X) >= 360.0f ? 0 : X;
		cam.transform.rotation = Quaternion::rotationAroundAxisVector(Y, worldUp);
		cam.transform.rotation = Quaternion::rotationAroundAxisVector(X, cam.transform.right()) * cam.transform.rotation.quaternion;
	}

	

	yPrev = y;
	xPrev = x;
}