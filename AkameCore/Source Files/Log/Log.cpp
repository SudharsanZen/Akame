#include"Core/Log/Log.h"
#pragma warning(push, 0)
	#pragma warning(disable : 26451)
	#pragma warning(disable : 26812)
	#pragma warning(disable : 26498)
	#pragma warning(disable : 26495)
	#include <spdlog/sinks/stdout_color_sinks.h>
	#include <spdlog/sinks/basic_file_sink.h>
#pragma warning(pop)


std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

void Log::Init()
{
	std::vector<spdlog::sink_ptr> logSinks;
	logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Engine.log", true));

	

	s_CoreLogger = std::make_shared<spdlog::logger>("ENGINE", begin(logSinks), end(logSinks));
	spdlog::register_logger(s_CoreLogger);
	s_CoreLogger->set_level(spdlog::level::trace);
	s_CoreLogger->flush_on(spdlog::level::trace);

	s_ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
	spdlog::register_logger(s_ClientLogger);
	s_ClientLogger->set_level(spdlog::level::trace);
	s_ClientLogger->flush_on(spdlog::level::trace);
}

std::shared_ptr<spdlog::logger>& Log::GetCoreLogger() 
{ 
	return s_CoreLogger;
}

std::shared_ptr<spdlog::logger>& Log::GetClientLogger() 
{ 
	return s_ClientLogger; 
}
