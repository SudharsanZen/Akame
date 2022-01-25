#pragma once
#include<string>
#include<Rendering/Texture.h>
enum ContentBrowserFlags
{
	ak_Open_Mode = 1,
	ak_Close_Mode = 1<<1,
	ak_Only_Show_given_files = 1<<2
};
inline ContentBrowserFlags operator|(ContentBrowserFlags a, ContentBrowserFlags b)
{
	return static_cast<ContentBrowserFlags>(static_cast<int>(a) | static_cast<int>(b));
}
class ContentBrowser
{
private:
	bool open;
	std::string windowTitle;
	std::string currPath;
	std::string prevPath;
	std::string selectedfileName;
	ContentBrowserFlags mode;
	void DrawContentHierarchyPannel();
	void DrawHierarchyNode(std::string path, int const& base_flags);
	void DrawContentPanel();
	int drwID = 0;
public:
	void setCurrentPath(std::string newPath);
	ContentBrowser(std::string windowName, ContentBrowserFlags mode, std::string path);
	bool Open();
	void DrawUI();
	std::string getSelectedFileName();

};

