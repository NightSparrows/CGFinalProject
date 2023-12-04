#pragma once

#include <TrashEngine/core/Base.h>

namespace TrashEngine {

	class Terrain
	{
	public:

		virtual ~Terrain() = default;

		// load height map automatically load the normal map
		virtual bool loadHeightMap(const std::string& filePath) = 0;



	};

}
