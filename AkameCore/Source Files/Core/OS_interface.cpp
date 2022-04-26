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
#include"core/Log/Log.h"
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

DirectoryWatcher::DirectoryWatcher(const char* dir):m_dir(dir)
{
	m_watch_dir = true;
	m_dir_changed = false;
#ifdef _WIN32
	for (int i = 0; i < sizeof(m_watcher_handle) / sizeof(HANDLE); i++)
	{
		m_watcher_handle[i] = NULL;
	}
#endif
}

bool DirectoryWatcher::is_watching()
{
	return m_watch_dir;
}

void  DirectoryWatcher::reset_state()
{
	m_dir_changed = false;
}

bool DirectoryWatcher::directory_changed()
{
	return m_dir_changed;
}

void DirectoryWatcher::terminate()
{
	m_watch_dir = false;
#ifdef _WIN32
	AK_ASSERT(m_watcher_handle[1] != NULL && "termation handle is null, can't signal event!");
	if (!SetEvent(m_watcher_handle[0]))
		ENGINE_CORE_CRITICAL("DirectoryWatcher::SetEvent Failed!");
#endif
}

void DirectoryWatcher::operator()()
{
#ifdef _WIN32
	m_watcher_handle[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_watcher_handle[1] = FindFirstChangeNotificationA(m_dir, false, FILE_NOTIFY_CHANGE_FILE_NAME);
	m_watcher_handle[2] = FindFirstChangeNotificationA(m_dir, false, FILE_NOTIFY_CHANGE_DIR_NAME);





	AK_ASSERT(m_watcher_handle[0] != NULL && "shome shing wong! terminate event handle is NULL!");
	AK_ASSERT(m_watcher_handle[1] != INVALID_HANDLE_VALUE && "shome shing wong! file name change notification handle is invalid!");
	AK_ASSERT(m_watcher_handle[2] != INVALID_HANDLE_VALUE && "shome shing wong! directory name change notification handle is invalid!");

	while (m_watch_dir)
	{

		std::cout << "waiting for notification...\n";
		DWORD status = WaitForMultipleObjects(3, m_watcher_handle, FALSE, INFINITE);

		switch (status)
		{
			case WAIT_OBJECT_0:
				std::cout << "closing file watcher";
				return;
				break;
			case WAIT_OBJECT_0 + 1:

				m_dir_changed = true;
				if (FindNextChangeNotification(m_watcher_handle[1]) == FALSE)
				{
					ExitProcess(GetLastError());
				}
				break;

			case WAIT_OBJECT_0 + 2:

				m_dir_changed = true;
				if (FindNextChangeNotification(m_watcher_handle[2]) == FALSE)
				{
					printf("\n ERROR: FindNextChangeNotification function failed.\n");
					ExitProcess(GetLastError());
				}
				break;
			default:
				ENGINE_CORE_ERROR("DirectoryWatcher::invalid status!");
				return;

		}

	}
#endif
}
