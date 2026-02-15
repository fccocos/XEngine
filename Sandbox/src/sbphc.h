#pragma once
// ========== 必须的基础头文件（解决所有 std::xxx 未定义问题） ==========
#include <iostream>       // 解决 std::ostream 未定义
#include <string>         // 解决 std::string 未定义
#include <functional>     // 解决 std::function 未定义
#include <cstdint>        // 解决 uintptr_t/intptr_t/ptrdiff_t 未定义
#include <cstdarg>        // 解决 va_list 未定义
#include <utility>        // 解决模板相关语法错误
#include <type_traits>    // 解决类型推导问题
#include <atomic>         // 解决 _invoke_watson 相关的 atomic 头文件错误
#include <memory>         // 解决 std::shared_ptr 相关问题

