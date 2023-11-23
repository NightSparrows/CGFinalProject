

workspace "CGFinalProject"
	architecture "x64"
	startproject "FinalProject"
	configurations { "Debug", "Release" }
	
	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "TrashEngineCore/vendor/glfw"
group ""

include "TrashEngineCore"
include "FinalProject"


	
	
	

