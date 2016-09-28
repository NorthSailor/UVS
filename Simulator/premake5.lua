
project "Simulator" 
	kind "ConsoleApp"
	language "C++"
	includedirs { "..",
		      "../3rdParty/glm",
		      "../3rdParty" }

	filter "system:linux"
		links { "PlanetScape", "FrameView", "ImGUI", "assimp",
			"GL", "GLEW", "SDL2", "pthread" }

	filter "system:macosx"
		links { "PlanetScape", "FrameView", "ImGUI", "assimp",
			"OpenGL.framework", "SDL2.framework" }

	targetname "UVS"
	files {
		"*.cpp",
		"*.h"
	}

