-- Global Solution Settings

-- OpenGL-Sandbox solution
workspace "OpenGL-Sandbox"
	architecture "x64"
	startproject "OpenGL-Sandbox"

	configurations
	{
		"Debug",
		"Release"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}



outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to OpenGL-Core -> Defined globally to be used by Other Lua Files
IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["Glad"] = "vendor/Glad/include"
IncludeDir["ImGui"] = "vendor/imgui"
IncludeDir["glm"] = "vendor/glm"
IncludeDir["stb_image"] = "vendor/stb_image"

-- Projects
-- group is a virtual folder for organizational purposes. Groups The following Dependencies into a virtual location
group "Dependencies" -- applied to sandbox project - includes lua files for the glfw,glad, and imgui libraries
	include "OpenGL-Core/vendor/GLFW"
	include "OpenGL-Core/vendor/Glad"
	include "OpenGL-Core/vendor/imgui"
group ""


-- specify lua script Files in the followig directories
include "OpenGL-Core"
include "OpenGL-Sandbox"

-----------------------------------------------------

-- AlgoVisCPP solution
workspace "AlgoVisCPP"
    startproject "AlgoVisCPP" -- set the starting project
    architecture "x64"
    startproject "AlgoVisCPP"

    configurations -- set to debug and release for All Configurations
    {
        "Debug",
        "Release"
    }

    flags -- use multithreading to compile project
    {
        "MultiProcessorCompile"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to OpenGL-Core
IncludeDir = {}
IncludeDir["GLFW"] = "vendor/GLFW/include"
IncludeDir["Glad"] = "vendor/Glad/include"
IncludeDir["ImGui"] = "vendor/imgui"
IncludeDir["glm"] = "vendor/glm"
IncludeDir["stb_image"] = "vendor/stb_image"

-- Projects 
group "Dependencies"
    includeexternal "OpenGL-Core/vendor/GLFW"
    includeexternal "OpenGL-Core/vendor/Glad"
    includeexternal "OpenGL-Core/vendor/imgui"
group ""

-- includeexternal will reference the opengl-core lua file but not regenerate it. It was already 
includeexternal "OpenGL-Core"
include "AlgoVisCPP"