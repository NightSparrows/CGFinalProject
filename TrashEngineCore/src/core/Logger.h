#pragma once

#include <TrashEngine/core/Base.h>

#include <spdlog/spdlog.h>

namespace TrashEngine {

	class Logger
	{
	private:
		NS_API static Ref<spdlog::logger> s_logger;

	public:
		NS_API static void Init();

		NS_API static Ref<spdlog::logger>& GetLogger() { return s_logger; }

	};

}

#ifdef NS_DEBUG
#define NS_CORE_TRACE(...) ::TrashEngine::Logger::GetLogger()->trace(__VA_ARGS__)
#else
#define NS_CORE_TRACE(...)
#endif

#define NS_CORE_INFO(...) ::TrashEngine::Logger::GetLogger()->info(__VA_ARGS__)
#define NS_CORE_WARN(...) ::TrashEngine::Logger::GetLogger()->warn(__VA_ARGS__)
#define NS_CORE_ERROR(...) ::TrashEngine::Logger::GetLogger()->error(__VA_ARGS__)
#define NS_CORE_CRITICAL(...) ::TrashEngine::Logger::GetLogger()->critical(__VA_ARGS__)

