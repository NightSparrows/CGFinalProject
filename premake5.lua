
newoption {
	trigger = "buildtype",
	description = "Engine build whether static or shared",
	allowed = {
		{"static"},
		{"shared"}
	},
	default = "shared"
}


include "Dependencies.lua"

workspace "CGFinalProject"
	architecture "x64"
	startproject "FinalProject"
	configurations { "Debug", "Release" }
	disablewarnings { "4819" }
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "TrashEngineCore/vendor/glfw"
	include "TrashEngineCore/vendor/imgui"
	include "TrashEngineCore/vendor/assimp"
group ""

include "FinalProject"
include "TrashEngineCore"

	
	
	

