workspace "XEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

    -- 全局强制设置：所有项目默认使用动态运行时库
    filter "system:windows"
        staticruntime "Off"  -- 全局禁用静态 CRT
        systemversion "latest"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "XEngine/vendor/GLFW/include"
IncludeDir["glad"] = "XEngine/vendor/glad/include"

include "XEngine/vendor/GLFW"
include "XEngine/vendor/glad"

------------------------------------------------------------------------
------------------------ XEngine 项目 -----------------------------------
-------------------------------------------------------------------------
project "XEngine"
    location "XEngine"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "xepch.h"
    pchsource "XEngine/src/xepch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}"
    }

    -- 只链接必要的库，CRT 库由编译选项自动链接（关键！）
    links {
        "GLFW",
        "glad",
        "opengl32.lib",
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS",
        "_CRT_NONSTDC_NO_DEPRECATE"
    }

    filter "system:windows"
        cppdialect "C++17"
        buildoptions "/utf-8"
        defines {
            "XE_PLATFORM_WINDOWS",
            "XE_BUILD_DLL",
            "GLFW_INCLUDE_NONE"
        }

    -- 构建后拷贝 DLL 到 Sandbox 输出目录（修正命令格式）
    postbuildcommands{
         -- 转义引号，确保路径含空格时正常执行；补充 /Y 覆盖文件
         ("{COPY} %{cfg.buildtarget.abspath} ../bin/" .. outputdir .. "/Sandbox")
    }

    filter "configurations:Debug"
        defines {"XE_DEBUG"}
        runtime "Debug"
        symbols "On"
        buildoptions "/MDd"  -- 强制使用动态调试 CRT
        -- 移除手动链接 CRT 库，让 VS 自动匹配

    filter "configurations:Release"
        defines {"XE_RELEASE"}
        runtime "Release"
        optimize "On"
        symbols "On"
        buildoptions "/MD"  -- 强制动态发布 CRT

    filter "configurations:Dist"
        defines {"XE_DIST"}
        runtime "Release"
        optimize "On"
        symbols "Off"
        buildoptions "/MD"

-----------------------------------------------------------------------------------
---------------------------------- Sandbox项目 -------------------------------------
-----------------------------------------------------------------------------------
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs {
        "XEngine/vendor/spdlog/include",
        "XEngine/src"
    }

    links { "XEngine" }

    filter "system:windows"
        cppdialect "C++17"
        buildoptions "/utf-8"
        defines {
            "XE_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        defines {"XE_DEBUG"}
        runtime "Debug"
        symbols "On"
        buildoptions "/MDd"  -- 和 XEngine 保持一致

    filter "configurations:Release"
        defines {"XE_RELEASE"}
        runtime "Release"
        optimize "On"
        symbols "On"
        buildoptions "/MD"

    filter "configurations:Dist"
        defines {"XE_DIST"}
        runtime "Release"
        optimize "On"
        symbols "Off"
        buildoptions "/MD"