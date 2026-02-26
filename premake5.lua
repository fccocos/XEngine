workspace "XEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

    filter "system:windows"
        staticruntime "On" 
        systemversion "latest"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "XEngine/vendor/GLFW/include"
IncludeDir["glad"] = "XEngine/vendor/glad/include"
IncludeDir["ImGui"] = "XEngine/vendor/imgui"
IncludeDir["glm"] = "XEngine/vendor/glm"
IncludeDir["stb_image"] = "XEngine/vendor/stb_image"

include "XEngine/vendor/GLFW"
include "XEngine/vendor/glad"
include "XEngine/vendor/imgui"

------------------------------------------------------------------------
------------------------ XEngine 项目 -----------------------------------
-------------------------------------------------------------------------
project "XEngine"
    location "XEngine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    buildoptions "/utf-8"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "xepch.h"
    pchsource "XEngine/src/xepch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        --"%{prj.name}/vendor/glm/glm/**.hpp",
        --"%{prj.name}/vendor/glm/glm/**.inl"
        "%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h"
    }

    defines {
        "_CRT_SECURE_NO_WARNINGS",
    }
    includedirs {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb_image}"
    }

    -- 只链接必要的库，CRT 库由编译选项自动链接（关键！）
    links {
        "GLFW",
        "glad",
        "ImGui",
        "opengl32.lib",
    }

    filter "system:windows"
        defines {
            "XE_PLATFORM_WINDOWS",
            "XE_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "XE_ENABLE_ASSERTS",
        }

    filter "configurations:Debug"
        defines {"XE_DEBUG"}
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines {"XE_RELEASE"}
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines {"XE_DIST"}
        runtime "Release"
        optimize "On"

-----------------------------------------------------------------------------------
---------------------------------- Sandbox项目 -------------------------------------
-----------------------------------------------------------------------------------
project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    buildoptions "/utf-8"
    staticruntime "On"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
    }

    includedirs {
        "XEngine/vendor/spdlog/include",
        "XEngine/src",
        "%{IncludeDir.glm}",  -- 新增：Sandbox 项目也能找到 GLM 头文件
        "%{IncludeDir.ImGui}"
    }

    links { "XEngine" }

    filter "system:windows"
        defines {
            "XE_PLATFORM_WINDOWS",
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        defines {"XE_DEBUG"}
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines {"XE_RELEASE"}
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines {"XE_DIST"}
        runtime "Release"
        optimize "On"