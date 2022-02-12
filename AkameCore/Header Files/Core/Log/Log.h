#pragma once
// This ignores all warnings raised inside External headers

#ifdef AK_EXPOSE_SPD_LOG
#pragma warning(push, 0)
#pragma warning(disable : 26451)
#pragma warning(disable : 26812)
#pragma warning(disable : 26498)
#pragma warning(disable : 26495)
	#include <spdlog/spdlog.h>
	#include <spdlog/fmt/ostr.h>
#pragma warning(pop)
#include"Core/AkameCore.h"


class Log
{
public:
	AKAME_API static void Init();

	AKAME_API static std::shared_ptr<spdlog::logger>& GetCoreLogger();
	AKAME_API static std::shared_ptr<spdlog::logger>& GetClientLogger();
private:
	AKAME_API static std::shared_ptr<spdlog::logger> s_CoreLogger;
	AKAME_API static std::shared_ptr<spdlog::logger> s_ClientLogger;
};


// Core log macros
#define ENGINE_CORE_TRACE(...)    Log::GetCoreLogger()->trace(__VA_ARGS__)
#define ENGINE_CORE_INFO(...)     Log::GetCoreLogger()->info(__VA_ARGS__)
#define ENGINE_CORE_WARN(...)     Log::GetCoreLogger()->warn(__VA_ARGS__)
#define ENGINE_CORE_ERROR(...)    Log::GetCoreLogger()->error(__VA_ARGS__)
#define ENGINE_CORE_CRITICAL(...) Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define ENGINE_TRACE(...)         Log::GetClientLogger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...)          Log::GetClientLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...)          Log::GetClientLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...)         Log::GetClientLogger()->error(__VA_ARGS__)
#define ENGINE_CRITICAL(...)      Log::GetClientLogger()->critical(__VA_ARGS__)
#endif