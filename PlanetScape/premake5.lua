
project "PlanetScape"
	kind "StaticLib"
	language "C++"
	includedirs {
	    "../3rdParty/glm/",
	    "../3rdParty/",
	    "../"
	}
	
	files {
		"*.cpp",
		"*.h"
	}
