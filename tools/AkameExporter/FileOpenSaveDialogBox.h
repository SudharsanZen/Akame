#pragma once
#include<iostream>
enum class FileOpenSaveDialogBoxFlag : uint64_t
{
	ak_Open_Mode				=	1 << 0,
	ak_Close_Mode				=	1 << 1,
	ak_Only_Show_given_files	=	1 << 2,
	ak_Folder_mode				=	1 << 3,
	ak_File_mode				=	1 << 4
};

FileOpenSaveDialogBoxFlag operator | (FileOpenSaveDialogBoxFlag a, FileOpenSaveDialogBoxFlag b);
FileOpenSaveDialogBoxFlag operator &(FileOpenSaveDialogBoxFlag a, FileOpenSaveDialogBoxFlag b);
class FileOpenSaveDialogBox
{
private:
	bool open;
	bool m_path_is_set;
	std::string windowTitle;
	std::string currPath;
	std::string prevPath;
	std::string selectedfileName;
	FileOpenSaveDialogBoxFlag m_mode;
	void DrawContentHierarchyPannel();
	void DrawHierarchyNode(std::string path, int const& base_flags);
	void DrawContentPanel();
	int drwID = 0;
	
public:
	void reset();
	std::string getCurrentPath();
	void setCurrentPath(std::string newPath);
	FileOpenSaveDialogBox(std::string windowName, FileOpenSaveDialogBoxFlag mode, std::string path);
	bool Open();
	bool isPathSet();
	void setOpen(bool openClose);
	void setMode(FileOpenSaveDialogBoxFlag mode);
	void DrawUI();
	std::string getSelectedFilePath();
	std::string getSelectedFolderPath();


};

