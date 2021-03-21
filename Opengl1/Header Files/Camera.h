#pragma once
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include"EngineMath.h"
class Camera
{
	
	GLboolean ortho;	//set true if camera is orthographic, set false for perspective
	
	glm::vec3 cameraTarget;
	//field of view ,screen aspec ratio, near plane, far plane
	GLfloat fovy, aspectRatio, nearz, farz;
	glm::vec3 camDir;
	inline void updateCamDir() { camDir = glm::normalize(cameraTarget-transform.position); }

public:
	Transform transform;//camera transform
	Camera();
	Camera(GLfloat fovy,GLfloat aspectRatio,GLfloat nearz,GLfloat farz,GLboolean ortho=false);

	void use();
	
	glm::vec3 getLookDir() { updateCamDir(); return camDir; };
	glm::vec3 lookDirRight() { updateCamDir(); return glm::cross(camDir,lookDirUp()); }
	glm::vec3 lookDirUp() 
	{
		updateCamDir(); 
		glm::vec3 up=glm::cross(camDir,worldRight);
		if (glm::length(up)==0)
		{
			up = glm::cross(camDir,worldForward);
		}
		return up;
	}


	void setCameraPosition(GLfloat x,GLfloat y,GLfloat z);
	void setCameraPosition(glm::vec3 pos);
	void setCameraRotation(glm::vec3 rot);
	void setCameraRotation(glm::quat rot) { transform.rotation.quaternion =rot; }
	glm::vec3 getCameraPosition() { return transform.position; };


	void setCameraTargetPosition(GLfloat x, GLfloat y, GLfloat z);
	void setCameraTargetPosition(glm::vec3 pos);
	glm::vec3 getCameraTargetPosition() { return cameraTarget; };

	inline void setFieldOfView(GLfloat degrees) { this->fovy = glm::radians(degrees); };
	inline void setAspectRation(GLfloat aspecR) { this->aspectRatio = aspecR; };

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

};

