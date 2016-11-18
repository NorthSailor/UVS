
project "ImGUI"
	kind "StaticLib"
	language "C++"
	files {
		"*.cpp",
		"*.h"
	}

	includedirs { ".." }	
	
	filter { "system:linux" }
	    includedirs { "/usr/include/SDL2" }
	


