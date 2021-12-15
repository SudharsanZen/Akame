#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<vector>
#include<stdlib.h>
#include<Core/Engine.h>
#include<Rendering/DeferredRendererFragmentBuffer.h>
#include<Core/Scene.h>
#include<sstream>
#include"Core/Debug/Debug.h"
#include"Core/Editor/EditorUI.h"
#define sizeX 10.0f
#define sizeY 10.0f

int main()
{
	AssetManager::setAssetRoot("../../../Assets/");
	std::string rootDir(AssetManager::getAssetRoot());
	Window window(800, 800, "DUMBSTUFF");
	
	if (!window.initialize())
	{
		std::cout << "Can't init WINDOW";
		return -1;
	}

	

	Scene scene(window);

	
	Editor editor(window,scene);
	scene.OnStart();
	scene.vsyncOn(true);
	float deltaTime = 0;

	while (!window.closeWindow())
	{
		
		deltaTime = scene.getDeltaTime();
		flyCam(scene.cam,scene.getDeltaTime());
		scene.cam.setAspectRation((float)window.getBufferWidth() / (float)window.getBufferHeight());
		scene.clearBuffer();
	
		scene.Render();
		editor.DrawUI();
		
	
		scene.swapBuffers();
		
		
	}



	return 0;
}





