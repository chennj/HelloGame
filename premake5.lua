workspace "HelloGame"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Import GLFW and GLAD---------------------------------------
IncludeDir = {}
IncludeDir["GLFW"] = "GameEngine/vendor/glfw/include"
IncludeDir["GLAD"] = "GameEngine/vendor/glad/include"

include "GameEngine/vendor/glfw"
include "GameEngine/vendor/glad"
-- ---------------------------------------------------
project "GameEngine"
	location "GameEngine"
	kind "SharedLib"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("temp/" .. outputdir .. "/%{prj.name}")

	pchheader "sopch.h"
	pchsource "GameEngine/src/sopch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"OpenGL32.Lib"
	}

	filter "system:windows"
		cppdialect "C++11"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SE_PLATFORM_WINDOWS",
			"SE_BUILD_DLL",
			--"SE_ENABLE_ASSERTS"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "SOME_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SOME_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SOME_DIST"
		optimize "On"

	filter {"system:windows","configurations:Debug"}
		buildoptions "/MDd"

	filter {"system:windows","configurations:Release"}
		buildoptions "/MD"

	filter {"system:windows","configurations:Dist"}
		buildoptions "/MD"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("temp/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"GameEngine/vendor/spdlog/include",
		"GameEngine/src"
	}

	links
	{
		"GameEngine"
	}

	filter "system:windows"
		cppdialect "C++11"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SANDBOX_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "SANDBOX_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "SANDBOX_DIST"
		optimize "On"

	filter {"system:windows","configurations:Debug"}
		buildoptions "/MDd"

	filter {"system:windows","configurations:Release"}
		buildoptions "/MD"

	filter {"system:windows","configurations:Dist"}
		buildoptions "/MD"

project "HelloGame"
	location "HelloGame"
	kind "ConsoleApp"
	language "C++"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("temp/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/**.h",
		"%{prj.name}/**.cpp"
	}
	removefiles { "%{prj.name}/common/imgui/main.cpp" }

	includedirs
	{
		"dependence/glew-2.1.0-win32/include",
		"dependence/glfw-3.3.2.bin.WIN64/include",
		"dependence"
	}

	libdirs
	{
		"dependence/glew-2.1.0-win32/lib/Release/x64",
		"dependence/glfw-3.3.2.bin.WIN64/lib-vc2015"
	}

	filter "system:windows"
		cppdialect "C++11"
		staticruntime "On"
		systemversion "latest"

	filter "configurations:Debug"
		defines "HELLOGAME_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "HELLOGAME_RELEASE"
		optimize "On"

	filter "configurations:Dist"
		defines "HELLOGAME_DIST"
		optimize "On"

	filter {"system:windows","configurations:Debug"}
		buildoptions "/MDd"

	filter {"system:windows","configurations:Release"}
		buildoptions "/MD"

	filter {"system:windows","configurations:Dist"}
		buildoptions "/MD"