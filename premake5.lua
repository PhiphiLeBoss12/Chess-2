-- Ripped from Cherno's video on premake
-- https://www.youtube.com/watch?v=sULV3aB2qeU

workspace "Chess2"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	startproject "Chess"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Chess"
	location "Chess"
	kind "ConsoleApp"
	language "C"
	cdialect "C17"
	
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c"
	}

	includedirs
	{
		"Chess/src",
		"Chess/src/Chess",
		"Chess/src/vendor"
	}

	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		libdirs ("libs/windows")

		links
		{
			"SDL2",
			"SDL2main"
		}

	filter "system:linux"
		staticruntime "On"
		systemversion "latest"
		libdirs ("libs/linux")

		links
		{
			"SDL2",
			"SDL2main"
		}

	filter "configurations:Debug"
		defines
		{
			"CHESS_DEBUG",
			"CHESS_DO_ASSERT",
			"CHESS_OPENGL_DEBUG"
		}
		symbols "On"

	filter "configurations:Release"
		defines
		{
			"CHESS_DEBUG",
			"CHESS_DO_ASSERT",
		}
		optimize "On"