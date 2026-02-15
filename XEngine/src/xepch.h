
// XEngine pre-compile header file


#pragma once

#include <memory>
#include <algorithm>
#include <functional>
#include <sstream>
#include <iostream>   // 解决 std::ostream 未定义
#include <cstdint>    // 解决 uintptr_t/intptr_t/ptrdiff_t 未定义
#include <cstdarg>    // 解决 va_list 未定义
#include <utility>    // 解决模板/通用工具类问题
#include <type_traits> // 解决类型推导相关问题

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>

#include "XEngine/log.h"

#ifdef XE_PLATFORM_WINDOWS

#include <Windows.h>

#endif
