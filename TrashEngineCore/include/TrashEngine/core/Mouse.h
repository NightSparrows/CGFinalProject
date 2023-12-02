#pragma once

#include <TrashEngine/deps/glm/glm.hpp>

#include "RenderWindow.h"

namespace TrashEngine {

	using MouseCode = uint16_t;

	class Mouse
	{
	public:
		Mouse(RenderWindow* window);

		NS_API bool isButtonDown(MouseCode button) const;

		// get the delta per frame
		NS_API glm::vec2 getDelta() const;

	private:
		RenderWindow* m_window;
	};

	namespace MouseButton {

		// From glfw3.h
		enum MouseCode
		{
			Left = 0,
			Right = 1,
			Middle = 2
		};
	}

}
