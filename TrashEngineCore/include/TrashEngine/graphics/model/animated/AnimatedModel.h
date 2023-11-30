#pragma once

#include <TrashEngine/core/Base.h>
#include "AnimatedModelData.h"

namespace TrashEngine {

	class AnimatedModel
	{
	public:
		virtual ~AnimatedModel() = default;

		/// <summary>
		/// Load model data form AnimatedModelData
		/// </summary>
		/// <param name="modelData"></param>
		/// <returns></returns>
		NS_API virtual bool loadData(AnimatedModelData* modelData) = 0;
	};

}
