#pragma once

#include "core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace XEngine {

	class XENGINE_API Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

// core log macros
#define XE_CORE_TRAC(...)             ::XEngine::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define XE_CORE_INFO(...)             ::XEngine::Log::GetCoreLogger()->info(__VA_ARGS__);
#define XE_CORE_WARN(...)             ::XEngine::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define XE_CORE_ERROR(...)            ::XEngine::Log::GetCoreLogger()->error(__VA_ARGS__);
#define XE_CORE_FATAL(...)            ::XEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__);

// client log macros
#define XE_TRAC(...)                ::XEngine::Log::GetClientLogger()->trace(__VA_ARGS__);
#define XE_INFO(...)                ::XEngine::Log::GetClientLogger()->info(__VA_ARGS__);
#define XE_WARN(...)                ::XEngine::Log::GetClientLogger()->warn(__VA_ARGS__);
#define XE_ERROR(...)               ::XEngine::Log::GetClientLogger()->error(__VA_ARGS__);
#define XE_FATAL(...)               ::XEngine::Log::GetClientLogger()->fatal(__VA_ARGS__);


