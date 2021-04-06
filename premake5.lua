workspace "HelloGame"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Import GLFW and GLAD and ImGui---------------------------------------
IncludeDir = {}
IncludeDir["GLFW"]		= "GameEngine/vendor/glfw/include"
IncludeDir["GLAD"]		= "GameEngine/vendor/glad/include"
IncludeDir["ImGui"]		= "GameEngine/vendor/imgui"
IncludeDir["glm"]		= "GameEngine/vendor/glm"
IncludeDir["stb_image"]	= "GameEngine/vendor/stb_image"

include "GameEngine/vendor/glfw"
include "GameEngine/vendor/glad"
include "GameEngine/vendor/imgui"
-- ---------------------------------------------------

project "GameEngine"
	location "GameEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("temp/" .. outputdir .. "/%{prj.name}")

	pchheader "sopch.h"
	pchsource "GameEngine/src/sopch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/assets/shaders/**.glsl",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"OpenGL32.Lib"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SE_PLATFORM_WINDOWS",
			"SE_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "SOME_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SOME_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SOME_DIST"
		runtime "Release"
		optimize "on"

	filter {"system:windows","configurations:Debug"}
		buildoptions "/MTd"

	filter {"system:windows","configurations:Release"}
		buildoptions "/MT"

	filter {"system:windows","configurations:Dist"}
		buildoptions "/MT"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++11"
	staticruntime "on"

	targetdir("bin/" .. outputdir .. "/%{prj.name}")
	objdir("temp/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl"
	}

	includedirs
	{
		"GameEngine/vendor/spdlog/include",
		"GameEngine/src",
		"GameEngine/vender",
		"%{IncludeDir.glm}"
	}

	links
	{
		"GameEngine"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"SE_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "SANDBOX_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "SANDBOX_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "SANDBOX_DIST"
		runtime "Release"
		optimize "on"

	filter {"system:windows","configurations:Debug"}
		buildoptions "/MTd"

	filter {"system:windows","configurations:Release"}
		buildoptions "/MT"

	filter {"system:windows","configurations:Dist"}
		buildoptions "/MT"

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
		buildoptions "/MTd"

	filter {"system:windows","configurations:Release"}
		buildoptions "/MT"

	filter {"system:windows","configurations:Dist"}
		buildoptions "/MT"