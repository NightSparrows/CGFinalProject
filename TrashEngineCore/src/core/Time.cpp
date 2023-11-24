
#include "nspch.h"
#include <TrashEngine/core/Time.h>

#include <GLFW/glfw3.h>

namespace TrashEngine {

	NS_API Time Time::GetTime()
	{
		return (float)glfwGetTime();
	}

}
