
workspace "UVS"
	configurations { "Debug", "Release" }
	location "Build"
	
	defines { "OPENGL" }
	buildoptions { "-g -Werror -Wall -Wextra -Wno-unused -std=c++14" }

-- If it is available, use Clang
if os.execute("clang -v") == 0 then
	toolset "clang"
end

include "FV"
include "Simulator"

