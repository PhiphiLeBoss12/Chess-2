-- Stolen from Cherno's video on premake
-- https://www.youtube.com/watch?v=sULV3aB2qeU

-- Defines the name of the workspace (the Visual Studio .sln file)
workspace "Chess2"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release"
	}

	startproject "Chess"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Defines the project (the .vcxproj file in Chess/)
project "Chess"
	location "Chess"
	kind "ConsoleApp"
	language "C"
	cdialect "C17"
	
	-- Defines the directory of the executable and the object files
	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin-intermediate/" .. outputDir .. "/%{prj.name}")

	-- Specifies the files to include in the project
	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/vendor/**.c",
	}

	-- Specifies the include directories
	includedirs
	{
		"Chess/src",
		"Chess/src/Chess",
		"Chess/src/vendor"
	}

	-- If the project is generated on Windows it will apply these rules
	filter "system:windows"
		staticruntime "On"
		systemversion "latest"
		libdirs ("libs/windows")
		-- Libraries to link
		links
		{
			"SDL2",
			"SDL2main"
		}
		postbuildcommands ("xcopy ..\\libs\\windows\\dlls\\ ..\\bin\\" .. outputDir .. "\\%{prj.name}\\ /s /e /y /i")

	-- If the project is generated on Linux it will apply these rules
	filter "system:linux"
		staticruntime "On"
		systemversion "latest"
		libdirs ("libs/linux")
		links
		{
			"SDL2",
			"SDL2main"
		}

	-- If the project is generated with the Debug configuration it will apply these rules
	filter "configurations:Debug"
		defines
		{
			"CHESS_DEBUG",
			"CHESS_DO_ASSERT"
		}
		symbols "On"

	-- If the project is generated with the Debug configuration it will apply these rules
	filter "configurations:Release"
		optimize "On"