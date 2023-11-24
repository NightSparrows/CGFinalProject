
project "FinalProject"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	defines {
		"NS_BUILD_SHARED"		-- build shared library
	}

	includedirs {
		"include"
	}

	files {
		"include/**.h",
		"src/**.h",
		"src/**.cpp"
	}

	includedirs {
		"%{IncludeDir.TrashEngineCore}",
	}

	links {
		"TrashEngineCore",
	}

	filter "system:windows"
		cppdialect "C++17"
		
	filter { "system:windows", "configurations:Debug" }
		runtime "Debug"
		
	filter { "system:windows", "configurations:Release" }
		runtime "Release"