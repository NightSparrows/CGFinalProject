

project "TrashEngineCore"
	language "C++"
	kind "SharedLib"
	staticruntime "off"
	
	defines {
		"NS_BUILD_SHARED",
		"NS_BUILD_ENGINE"				-- engine compile definition
	}
	
	-- copy to finalProject after compiled.
	postbuildcommands ("{COPY} %{cfg.buildtarget.relpath} %{wks.location}/bin/" .. outputdir .. "/FinalProject")
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	includedirs {
		"include",
		"src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.spdlog}"
	}

	files {
		"include/**.h",
		"src/**.h",
		"src/**.cpp",
		"src/**.c"
	}

	pchheader "nspch.h"
	pchsource "src/nspch.cpp"
	
	flags {
		"MultiProcessorCompile"
	}

	links {
		"GLFW"
	}

	

