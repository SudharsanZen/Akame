#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"EngineMath.h"

class Camera
{
	
	GLboolean ortho;	//set true if camera is orthographic, set false for perspective
	
	//glm::vec3 cameraTarget;
	//field of view ,screen aspec ratio, near plane, far plane
	GLfloat fovy, aspectRatio, nearz, farz;
	glm::vec3 camDir;
	//inline void updateCamDir() { camDir = glm::normalize(cameraTarget-transform.position); }

public:
	Transform transform;//camera transform
	Camera();
	Camera(GLfloat fovy,GLfloat aspectRatio,GLfloat nearz,GLfloat farz,GLboolean ortho=false);


	glm::mat4 lookAt(Transform& transform, glm::vec3& pos);
	
	void setCameraPosition(GLfloat x,GLfloat y,GLfloat z);
	void setCameraPosition(glm::vec3 pos);
	void setCameraRotation(glm::vec3 rot);
	void setCameraRotation(glm::quat rot) { transform.rotation.quaternion =rot; }
	glm::vec3 getCameraPosition() { return transform.position; };

	//sets Field of View for the camera. takes input in degrees.
	inline void setFieldOfView(GLfloat degrees) { this->fovy = glm::radians(degrees); };
	//sets Aspect ration of the window to render in proper proportions.
	inline void setAspectRation(GLfloat aspecR) { this->aspectRatio = aspecR; };

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

};

//A default implementation of a flycam.
void flyCam(Camera& cam,float deltaTime);