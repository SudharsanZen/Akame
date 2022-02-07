#pragma once
#include<string>
#include<Rendering/Texture.h>

class AKAME_API ContentBrowser
{
private:
	bool open;
	std::string windowTitle;
	std::string currPath;
	std::string prevPath;
	std::string selectedfileName;

	void DrawContentHierarchyPannel();
	void DrawHierarchyNode(std::string path, int const& base_flags);
	void DrawContentPanel();
	int drwID = 0;
public:
	void setCurrentPath(std::string newPath);
	ContentBrowser(std::string windowName, std::string path);
	bool Open();
	void DrawUI();
	std::string getSelectedFileName();

};

