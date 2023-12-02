#include "nspch.h"
#include <TrashEngine/core/Mouse.h>
#include "GLFWRenderWindow.h"

namespace TrashEngine {

	Mouse::Mouse(RenderWindow* window) :
		m_window(window)
	{
	}

	NS_API bool Mouse::isButtonDown(MouseCode button) const
	{
		auto state = glfwGetMouseButton(static_cast<GLFWRenderWindow*>(this->m_window)->getHandle(), static_cast<int32_t>(button));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	NS_API glm::vec2 Mouse::getDelta() const
	{
		return static_cast<GLFWRenderWindow*>(this->m_window)->getWindowData()->mouseDeltaPosition;
	}

}
