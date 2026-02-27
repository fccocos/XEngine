#pragma once
#include <memory>

// Platform detection using predefined macros
#ifdef _WIN32
	/* Windows X64/X86 */
#	ifdef _WIN64
		/* Windows X64 */
#		define HZ_PLAFROM_WINDOWS
#	else
#		error "x86 Builds are not supported!"
#   endif
#elif defined(__APPLE__) || defined(__MACH__)
#	inlcude <TragetConditionals.h>
	/* 
	* TARGET_OS_MAC exists on all the platforms
	* so we must check all of them (in this order)
	* to ensure that we're running on MAC
	* and not some other Apple platform
	*/
#	if TARGET_IPHONE_SIMULATOR == 1
#		error "IOS simulator is not supported"
#	elif TARGET_OS_IPHONE == 1
#		define HZ_PLATFORM_IOS
#		error "IOS is not supported"
#	elif TARGET_OS_MAC == 1
#		define HZ_PLATFORM_MACOS
#		error "MacOS is not supported"
#	else
#		error "Unkown Apple platform"
#	endif
#endif


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

namespace XEngine {

	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using Ref = std::shared_ptr<T>;

}