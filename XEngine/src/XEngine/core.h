#pragma once

#ifdef XE_PLATFORM_WINDOWS
#	ifdef XE_BUILD_DLL
#		define XENGINE_API _declspec(dllexport)
#	else
#		define XENGINE_API _declspec(dllimport)
#	endif
#else
#	error "XEngine only surpport Windows"
#endif // XE_PLATFORM_WINDOWS

