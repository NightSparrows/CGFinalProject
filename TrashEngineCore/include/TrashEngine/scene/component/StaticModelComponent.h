#pragma once

#include <TrashEngine/core/Base.h>
#include <TrashEngine/graphics/StaticModel.h>

namespace TrashEngine {

	struct StaticModelComponent
	{
		Ref<StaticModel> model;

		StaticModelComponent(Ref<StaticModel> model)
		{
			this->model = model;
		}

		StaticModelComponent() = default;
		StaticModelComponent(const StaticModelComponent&) = default;

	};

}
