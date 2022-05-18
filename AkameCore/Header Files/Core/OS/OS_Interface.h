#pragma once

#include"Core/AkameCore.h"
#if defined(EXPOSE_OS_INTERFACE) || defined(AK_EXPORT)
	AKAME_API std::string SaveFileDialog(const char* filter, const char* defaultExt);
	AKAME_API std::string OpenFileDialog(const char* filter, const char* initdir);
	AKAME_API std::string BrowseSHFolderDialog(const char* initdir);
	AKAME_API std::string SelectFolderDialog(const char* initdir);
	AKAME_API void OpenInFileBrowser(const char* dir);
	class  AKAME_API DirectoryWatcher
	{
		bool	m_dir_changed;
		bool	m_watch_dir;
		void*	m_watcher_handle[3];
		const char*	m_dir;

	public:
		DirectoryWatcher(const char* dir);
		bool is_watching();
		void reset_state();
		bool directory_changed();
		void terminate();
		void operator()();
	};
#endif
