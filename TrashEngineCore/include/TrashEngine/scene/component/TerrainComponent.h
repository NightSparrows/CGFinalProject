#pragma once

#include <TrashEngine/graphics/model/terrain/Terrain.h>

namespace TrashEngine {

	class TerrainComponent
	{
	public:

		Ref<Terrain> terrain;

		TerrainComponent(Ref<Terrain> terrain)
		{
			this->terrain = terrain;
		}

		TerrainComponent() = default;
		TerrainComponent(const TerrainComponent&) = default;


	};

}
