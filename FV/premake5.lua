
project "FrameView"
	kind "StaticLib"
	language "C++"
	includedirs { "../3rdParty/assimp/include",
		      "../3rdParty/glm",
		      "../3rdParty" }
	files {
		"*.cpp",
		"*.h",
		"../3rdParty/stbi/*"
	}
	links { "assimp", "GLEW", "GL" }

