#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/graphics/StaticModelData.h>
#include <TrashEngine/graphics/model/animated/AnimatedModelData.h>
#include <TrashEngine/graphics/model/animated/AnimatedModelAnimation.h>

namespace TrashEngine {

	class ModelLoader
	{
	public:
		NS_API static Ref<StaticModelData> LoadStaticModelFromFile(const std::string& filePath, const std::string& texturePath);

		NS_API static Ref<AnimatedModelData> LoadAnimatedModelFromFile(const std::string& filePath, const std::string& texturePath);

		NS_API static Ref<AnimatedModelAnimation> LoadAnimatedModelAnimationFromFile(const std::string& filePath);

	};

}
