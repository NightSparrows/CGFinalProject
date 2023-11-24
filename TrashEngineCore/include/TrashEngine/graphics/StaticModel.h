#pragma once

#include <TrashEngine/core/Base.h>
#include "StaticModelData.h"

namespace TrashEngine {

	class StaticModel
	{
	public:
		virtual ~StaticModel() = default;

		/// <summary>
		/// Load model data form StaticModelData
		/// </summary>
		/// <param name="modelData"></param>
		/// <returns></returns>
		NS_API virtual bool loadData(StaticModelData* modelData) = 0;

	};

}
