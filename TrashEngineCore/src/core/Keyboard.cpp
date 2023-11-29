#include "nspch.h"
#include <TrashEngine/core/Keyboard.h>
#include <core/GLFWRenderWindow.h>

namespace TrashEngine {



	Keyboard::Keyboard(RenderWindow* window) :
		m_window(window)
	{
	}

	NS_API bool Keyboard::isKeyDown(KeyCode key) const
	{
		auto state = glfwGetKey(static_cast<GLFWRenderWindow*>(this->m_window)->getHandle(), key);
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}


}
