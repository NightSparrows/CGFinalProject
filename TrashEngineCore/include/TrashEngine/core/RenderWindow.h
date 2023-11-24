#pragma once

#include "Base.h"

namespace TrashEngine {

	class RenderWindow
	{
	public:
		virtual ~RenderWindow() = default;

		NS_API virtual uint32_t getWidth() const = 0;

		NS_API virtual uint32_t getHeight() const = 0;

	};

}
