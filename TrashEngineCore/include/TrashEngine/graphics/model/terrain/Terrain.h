#pragma once

#include <TrashEngine/core/Base.h>

#include "TerrainMaterialData.h"

namespace TrashEngine {

	class Terrain
	{
	public:

		virtual ~Terrain() = default;

		// load height map automatically load the normal map
		virtual bool loadHeightMap(const std::string& filePath) = 0;

		virtual bool loadMaterial(const TerrainMaterialData& materialData) = 0;

	};

}
