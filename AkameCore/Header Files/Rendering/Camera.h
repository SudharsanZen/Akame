#pragma once

#include"Math/EngineMath.h"
#include"Core/AkameCore.h"
typedef unsigned char GLboolean;
class Camera
{
	
	GLboolean ortho;	//set true if camera is orthographic, set false for perspective
	
	//glm::vec3 cameraTarget;
	//field of view ,screen aspec ratio, near plane, far plane
	float fovy, aspectRatio, nearz, farz;
	glm::vec3 camDir;
	//inline void updateCamDir() { camDir = glm::normalize(cameraTarget-transform.position); }

public:
	Transform transform;//camera transform
	AKAME_API Camera();
	AKAME_API Camera(float fovy,float aspectRatio,float nearz,float farz,GLboolean ortho=false);


	AKAME_API glm::mat4 lookAt(Transform& transform, glm::vec3& pos);
	
	AKAME_API void setCameraPosition(float x,float y,float z);
	AKAME_API void setCameraPosition(glm::vec3 pos);
	AKAME_API void setCameraRotation(glm::vec3 rot);
	AKAME_API void setCameraRotation(glm::quat rot) { transform.SetGlobalRotation(rot); }
	AKAME_API glm::vec3 getCameraPosition();;

	//sets Field of View for the camera. takes input in degrees.
	AKAME_API void setFieldOfView(float degrees);;
	//sets Aspect ration of the window to render in proper proportions.
	AKAME_API void setAspectRation(float aspecR);;

	AKAME_API glm::mat4 getViewMatrix();
	AKAME_API glm::mat4 getProjectionMatrix();
	AKAME_API float getNear();
	AKAME_API float getFar();
	AKAME_API void setNear(float n);
	AKAME_API void setFar(float f);
	AKAME_API float getFov();
	AKAME_API float getAspectRatio();


};

//A default implementation of a flycam.
AKAME_API void flyCam(Camera& cam,float deltaTime);