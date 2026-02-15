project "glad"
	kind "StaticLib"
	language "C"
	--staticruntime "off"
	--warnings "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	-- 关键新增：GLFW 静态编译必需定义 + 禁用 assert 消除 _wassert
	--defines {
	--	"GLFW_BUILD_STATIC",  -- 静态库编译标记（核心）
	--	"_CRT_SECURE_NO_WARNINGS"  -- 禁用 MSVC 安全警告，避免符号替换
	--}

	files {
		"include/glad/glad.h",
		"include/KHR/Khrplatform.h",
		"src/glad.c",
	}

	includedirs{
		"include"
	}

	
	filter "system:windows"
	    systemversion "latest"
		staticruntime "On"
	

	filter { "system:windows", "configurations:Release" }	
		buildoptions "/MT"

	