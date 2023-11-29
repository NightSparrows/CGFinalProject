#pragma once

#include <vector>

#include "MaterialData.h"
#include <TrashEngine/deps/glm/glm.hpp>

namespace TrashEngine {

	class StaticModelData
	{
	public:

		struct VertexData
		{
			glm::vec3 position;
			glm::vec3 normal;
			glm::vec3 tangent;
			glm::vec2 texCoord;
		};

		struct MeshData
		{
			uint32_t materialIndex;		// the index of the material in materials
			uint32_t indexOffset;	// 原則上用不到
			uint32_t indexCount;
		};

		std::vector<VertexData> vertices;
		std::vector<uint32_t> indices;

		std::vector<MeshData> meshes;

		std::vector<MaterialData> materials;

	};

}
