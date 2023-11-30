#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <TrashEngine/core/Base.h>
#include <TrashEngine/graphics/StaticModelData.h>
#include <TrashEngine/graphics/model/animated/AnimatedModelData.h>
#include <TrashEngine/graphics/model/animated/AnimatedModelAnimation.h>

namespace TrashEngine {

	class AssimpLoader
	{
	public:

		static Ref<StaticModelData> LoadStaticModelFromFile(const std::string& filePath, const std::string& texturePath);

		static Ref<AnimatedModelData> LoadAnimatedModelFromFile(const std::string& filePath, const std::string& texturePath);

		static Ref<AnimatedModelAnimation> LoadAnimatedModelAnimationFromFile(const std::string& filePath);

	protected:

		static bool ProcessStaticMeshes(const aiScene* aScene, Ref<StaticModelData> modelData);

		static bool ProcessAnimatedMeshes(const aiScene* aScene, Ref<AnimatedModelData> modelData);

		static void ProcessAnimatedJoint(const aiNode* node, Ref<AnimatedModelData> modelData, Ref<AnimatedModelJointData> jointData);

		static void ProcessMaterial(const aiScene* aScene, std::vector<MaterialData>& materialList, const std::string& texturePath);

		static std::string ProcessTexture(const aiMaterial* aMaterial, aiTextureType textureType, const std::string& texturePath);



	};

}
