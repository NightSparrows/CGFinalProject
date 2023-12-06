#pragma once

#include <vector>

#include <TrashEngine/graphics/MaterialData.h>

namespace TrashEngine {

	struct TerrainMaterialData
	{
	public:

		std::string blendMap;

		// r g b back
		std::vector<MaterialData> materialData;

	};

}
