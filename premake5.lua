workspace "HelloGame"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
		"%{prj.name}/vendor/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++11"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"SO_PLATFORM_WINDOWS",
			"SO_BUILD_DLL"
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
			"SO_PLATFORM_WINDOWS"
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
