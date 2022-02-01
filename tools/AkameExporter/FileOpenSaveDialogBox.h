#pragma once
#include<iostream>
enum FileOpenSaveDialogBoxFlag_
{
	ak_Open_Mode = 1,
	ak_Close_Mode = 1 << 1,
	ak_Only_Show_given_files = 1 << 2,
	ak_Folder_mode=1<<3,
	ak_File_mode=1<<4
};
typedef uint64_t FileOpenSaveDialogBoxFlag;

inline FileOpenSaveDialogBoxFlag operator|(FileOpenSaveDialogBoxFlag_ a, FileOpenSaveDialogBoxFlag_ b)
{
	return (static_cast<uint64_t>(a) | static_cast<uint64_t>(b));
}
class FileOpenSaveDialogBox
{
private:
	bool open;
	bool m_path_is_set;
	std::string windowTitle;
	std::string currPath;
	std::string prevPath;
	std::string selectedfileName;
	uint64_t m_mode;
	void DrawContentHierarchyPannel();
	void DrawHierarchyNode(std::string path, int const& base_flags);
	void DrawContentPanel();
	int drwID = 0;
	
public:
	void reset() { open = false; m_path_is_set = false; }
	std::string getCurrentPath() { return currPath; }
	void setCurrentPath(std::string newPath);
	FileOpenSaveDialogBox(std::string windowName, FileOpenSaveDialogBoxFlag mode, std::string path);
	bool Open();
	bool isPathSet() { return m_path_is_set; }
	void setOpen(bool openClose) { open = openClose; }
	void setMode(FileOpenSaveDialogBoxFlag mode) { m_mode = mode; }
	void DrawUI();
	std::string getSelectedFilePath();
	std::string getSelectedFolderPath();


};

