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


inline const char* get_filename(const char* full_path) {
	// 兼容 Windows(\) 和 Linux(/) 路径分隔符
	const char* last_slash = strrchr(full_path, '/');
	const char* last_backslash = strrchr(full_path, '\\');
	const char* sep = (last_backslash && last_backslash > last_slash) ? last_backslash : last_slash;
	return sep ? (sep + 1) : full_path;
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

#define LOG_TRACE_SHORT(...)        ::XEngine::Log::GetCoreLogger()->trace("[{}:{}] {}", get_filename(__FILE__), __LINE__, fmt::format(__VA_ARGS__))
#define LOG_DEBUG_SHORT(...)        ::XEngine::Log::GetCoreLogger()->debug("[{}:{}] {}", get_filename(__FILE__), __LINE__, fmt::format(__VA_ARGS__))
#define LOG_INFO_SHORT(...)         ::XEngine::Log::GetCoreLogger()->info("[{}:{}] {}", get_filename(__FILE__), __LINE__, fmt::format(__VA_ARGS__))
#define LOG_WARN_SHORT(...)         ::XEngine::Log::GetCoreLogger()->warn("[{}:{}] {}", get_filename(__FILE__), __LINE__, fmt::format(__VA_ARGS__))
#define LOG_ERROR_SHORT(...)        ::XEngine::Log::GetCoreLogger()->error("[{}:{}] {}", get_filename(__FILE__), __LINE__, fmt::format(__VA_ARGS__))
#define LOG_CRITICAL_SHORT(...)     ::XEngine::Log::GetCoreLogger()->critical("[{}:{}] {}", get_filename(__FILE__), __LINE__, fmt::format(__VA_ARGS__))


