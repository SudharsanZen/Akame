#pragma once
#include"Rendering/Shader.h"
#include"Rendering/Camera.h"
#include<ECS.h>
#include<string>
#include "Rendering/System/RenderingSystem.h"
/*This class is a base class that provides the 
*interface for defining call backs like
*preRender and post render actions for 
*a particular shader.*/
class ShaderRenderPipeline
{
	
	
public:

	//called everytime the windows is resized
	virtual void WindowsResizeCallBacks(int height,int width){
	}

	//called just before the rendering of entities begins
	virtual void OnPreRender(std::shared_ptr<Shader> shader,RenderingSystem *rsys,Camera cam){
	}

	//called after rendering stuff
	virtual void OnPostRender(std::shared_ptr<Shader> shader,RenderingSystem *rsys, Camera cam){
	}
	//destructor 
	virtual ~ShaderRenderPipeline(){
	}

};

