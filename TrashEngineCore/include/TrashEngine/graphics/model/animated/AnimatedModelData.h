#pragma once

#include <vector>

#include <TrashEngine/graphics/MaterialData.h>
#include <TrashEngine/deps/glm/glm.hpp>
#include <TrashEngine/deps/glm/gtc/quaternion.hpp>
#include "AnimatedModelJointData.h"

namespace TrashEngine {

	class AnimatedModelData
	{
	public:

		struct VertexData
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec3 tangent;
			glm::vec2 texCoord;
			glm::ivec4 boneID;
			glm::vec4 weight;
		};

		struct BoneData
		{
			std::string name;
			glm::mat4 offsetMatrix{glm::identity<glm::mat4>()};
		};

		struct MeshData
		{
			uint32_t materialIndex;		// the index of the material in materials
			uint32_t indexOffset;	// 原則上用不到
			uint32_t indexCount;
		};

		std::vector<VertexData> vertices;
		std::vector<BoneData> bones;
		Ref<AnimatedModelJointData> rootJoint;
		std::vector<uint32_t> indices;

		std::vector<MeshData> meshes;

		std::vector<MaterialData> materials;

	};

}
