
project "Simulator" 
	kind "ConsoleApp"
	language "C++"
	includedirs { "..",
		      "../3rdParty/glm" }
	links { "FrameView", "assimp", "GL", "GLEW", "SDL2", "pthread" }
	targetname "UVS"
	files {
		"*.cpp",
		"*.h"
	}

