#pragma once

#include <vector>

#include <TrashEngine/graphics/MaterialData.h>

namespace TrashEngine {

	struct TerrainMaterialData
	{
	public:

		std::string blendMap;

		std::vector<MaterialData> materialData;

	};

}
