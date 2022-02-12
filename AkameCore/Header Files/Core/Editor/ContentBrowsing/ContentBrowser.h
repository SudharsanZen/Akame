#pragma once
#include<string>
#include<Rendering/Texture.h>
#include"Core/Editor/AkameWindowBase.h"
class ContentBrowser:public AkameWindowBase
{
private:
	bool open;
	std::string windowTitle;
	std::string currPath;
	std::string prevPath;
	std::string selectedfileName;

	AKAME_API void DrawContentHierarchyPannel();
	AKAME_API void DrawHierarchyNode(std::string path, int const& base_flags);
	AKAME_API void DrawContentPanel();
	int drwID = 0;
public:
	AKAME_API void setCurrentPath(std::string newPath);
	AKAME_API ContentBrowser(std::string windowName, std::string path);
	AKAME_API bool Open();
	AKAME_API void DrawUI();
	AKAME_API std::string getSelectedFileName();

};

