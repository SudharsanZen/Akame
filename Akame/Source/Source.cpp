#define EDITOR_MODE
#include<iostream>
#include"Core/Engine.h"
#include"Core/Scene.h"
#include"Core/Window.h"
#include"Core/Debug/Debug.h"
#include<Rendering/DeferredRendererFragmentBuffer.h>
#include"Core/Serialization/SceneSerialization.h"

int main()
{


	AssetManager::setAssetRoot("../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(1080, 720, "testWindow");

	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}
	

	Scene scene(window);	
	Editor e(scene);
	scene.OnStart();
	scene.vsyncOn(false);
	scene.backGroundColor(1, 1, 1, 1);
	scene.backGroundColor(0.1f,0.1f,0.1f,0.1f);
	
	while (!window.closeWindow())
	{
		scene.clearBuffer();
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		e.DrawUI();
		scene.swapBuffers();

	}



	return 0;
}





