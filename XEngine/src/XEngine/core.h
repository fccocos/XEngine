#pragma once

#ifdef XE_PLATFORM_WINDOWS
#	if XE_DYNAMIC_LINK
#		pragma execution_character_set("utf-8")
#		ifdef XE_BUILD_DLL
#			define XENGINE_API _declspec(dllexport)
#		else
#			define XENGINE_API _declspec(dllimport)
#		endif // XE_BUILD_DLL
#	else
#		define XENGINE_API
#	endif // XE_DYNAMIC_LINK
#else
#	error "XEngine only supports Windows"
#endif // XE_PLATFORM_WINDOWS

#ifdef XE_ENABLE_ASSERTS
#	define XE_ASSERT(x, ...) { if(!(x)) { XE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#	define XE_CORE_ASSERT(x, ...) { if(!(x)) { XE_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
#	define XE_ASSERT(x, ...) 
#   define XE_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (0x01<<(x))

#define EX_BIND_EVENT_FN(fn)  std::bind(&fn, this, std::placeholders::_1) // 事件绑定函数