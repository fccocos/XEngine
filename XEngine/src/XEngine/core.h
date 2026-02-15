#pragma once

#ifdef XE_PLATFORM_WINDOWS
#pragma execution_character_set("utf-8")
#	ifdef XE_BUILD_DLL
#		define XENGINE_API _declspec(dllexport)
#	else
#		define XENGINE_API _declspec(dllimport)
#	endif
#else
#	error "XEngine only surpport Windows"
#endif // XE_PLATFORM_WINDOWS

#ifdef XE_ENABLE_ASSERTS
#	define XE_ASSERT(x, ...) { if(!(x)) { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#	define XE_CORE_ASSERT(x, ...) { if(!(x)) { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#	define XE_ASSERT(x, ...) 
#   define XE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (0x01<<(x))