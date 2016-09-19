
project "Simulator" 
	kind "ConsoleApp"
	language "C++"
	includedirs { "..",
		      "../3rdParty/glm",
		      "../3rdParty" }
	links { "PlanetScape", "FrameView","ImGUI", "assimp", "GL", "GLEW", "SDL2", "pthread" }
	targetname "UVS"
	files {
		"*.cpp",
		"*.h"
	}

