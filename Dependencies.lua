
-- Hazel Dependencies

VULKAN_SDK = os.getenv("VULKAN_SDK")

IncludeDir = {}

IncludeDir["TrashEngineCore"] = "%{wks.location}/TrashEngineCore/include"

IncludeDir["GLFW"] = "%{wks.location}/TrashEngineCore/vendor/glfw/include"
IncludeDir["spdlog"] = "%{wks.location}/TrashEngineCore/vendor/spdlog/include"