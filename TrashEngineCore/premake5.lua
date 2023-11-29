

project "TrashEngineCore"
	language "C++"
	kind "SharedLib"
	staticruntime "off"
	cppdialect "C++17"
	
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
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.imgui}"
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
		"GLFW",
		"imgui"
	}

	filter "configurations:Debug"
		defines "NS_DEBUG"
		symbols "On"

	filter "system:windows"
		systemversion "latest"
		-- ignoredefaultlibraries { "msvctd" }

	filter { "system:windows", "configurations:Debug" }
		runtime "Debug"
		
	filter { "system:windows", "configurations:Release" }
		runtime "Release"

