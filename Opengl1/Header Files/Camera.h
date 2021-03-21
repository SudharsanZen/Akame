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

	void use();
	/*glm::mat4 lookAt(Transform& transform, glm::vec3 &position)
	{
		glm::mat4 mat(0);
		mat[0] = glm::vec4(transform.right(), -position.x);
		mat[1] = glm::vec4(transform.up(), -position.y);
		mat[2] = glm::vec4(transform.forward(), -position.z);
		mat[3] = glm::vec4(0, 0, 0, 1);
		float* f = glm::value_ptr(mat);
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				std::cout << "[" << mat[i][j] << "] ";
			}
			std::cout << std::endl;
			std::cout << std::endl;

		}

		return mat;
	}*/

	glm::mat4 lookAt(Transform &transform,glm::vec3 &pos)
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
	/*glm::vec3 getLookDir() { updateCamDir(); return camDir; };
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
	}*/


	void setCameraPosition(GLfloat x,GLfloat y,GLfloat z);
	void setCameraPosition(glm::vec3 pos);
	void setCameraRotation(glm::vec3 rot);
	void setCameraRotation(glm::quat rot) { transform.rotation.quaternion =rot; }
	glm::vec3 getCameraPosition() { return transform.position; };


	//void setCameraTargetPosition(GLfloat x, GLfloat y, GLfloat z);
	//void setCameraTargetPosition(glm::vec3 pos);
	//glm::vec3 getCameraTargetPosition() { return cameraTarget; };

	inline void setFieldOfView(GLfloat degrees) { this->fovy = glm::radians(degrees); };
	inline void setAspectRation(GLfloat aspecR) { this->aspectRatio = aspecR; };

	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

};

