#include"Core/OS/OS_Interface.h"
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include"GLFW/glfw3.h"
#include<GLFW/glfw3native.h>
#include<string>
#include<wchar.h>
#include<locale>
#include<codecvt>
#include<iostream>
#ifdef _WIN32
#include<Windows.h>
#endif // _WIN32

#define CHARPTR2WCHARPTR(W_CHAR_PTR,CHAR_PTR)\
wcscpy(W_CHAR_PTR, std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(CHAR_PTR).c_str())

#define WCHARPTR2STRING(W_CHAR_PTR)\
std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(W_CHAR_PTR)

std::string SaveFileDialog(const char *filter,const char *defaultExt)
{
#ifdef _WIN32
	OPENFILENAMEA ofn;
	char szFile[512] = { 0 };
	
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = ".";
	ofn.lpstrDefExt = defaultExt;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	if (GetSaveFileNameA(&ofn))
	{
		
		return szFile;

	}

#endif // _WIN32
	return "";
}

std::string OpenFileDialog(const char* filter, const char* initdir)
{
#ifdef _WIN32
	OPENFILENAMEA ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	char szFile[512] = {};

	
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = GetActiveWindow();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = nullptr;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = initdir ? initdir : ".";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		return szFile;
	}
#endif // _WIN32
	return "";
}