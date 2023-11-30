#include "nspch.h"

#include <TrashEngine/loader/ModelLoader.h>

#include "AssimpLoader.h"

namespace TrashEngine {



	Ref<StaticModelData> ModelLoader::LoadStaticModelFromFile(const std::string& filePath, const std::string& texturePath)
	{
		// use assimp first
		return AssimpLoader::LoadStaticModelFromFile(filePath, texturePath);
	}

	NS_API Ref<AnimatedModelData> ModelLoader::LoadAnimatedModelFromFile(const std::string& filePath, const std::string& texturePath)
	{
		return AssimpLoader::LoadAnimatedModelFromFile(filePath, texturePath);
	}

	NS_API Ref<AnimatedModelAnimation> ModelLoader::LoadAnimatedModelAnimationFromFile(const std::string& filePath)
	{
		return AssimpLoader::LoadAnimatedModelAnimationFromFile(filePath);
	}

}
