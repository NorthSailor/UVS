
-- Automatic system detection does not work, just set the most common for now.
system "macosx"

workspace "UVS"
	configurations { "Debug", "Release" }
	location "Build"
	
	filter "system:linux"
		defines { }
		buildoptions { "-g -Werror -Wall -Wextra -Wno-unused -std=c++14" }
	
	filter "system:macosx"
		defines { "MACOS" }
		buildoptions { "-F /Library/Frameworks",
		        "-F ~/Library/Frameworks",
		        "-g -Werror -Wall -Wextra -std=c++14" }
		linkoptions { "-F/Library/Frameworks -L/usr/local/lib" }

	filter { }	
		sysincludedirs { "/usr/local/include" }
		syslibdirs { "/usr/local/lib" }
	

-- If it is available, use Clang
if os.execute("clang -v") == 0 then
	toolset "clang"
end

include "FV"
include "3rdParty/imgui"
include "PlanetScape"
include "Simulator"

