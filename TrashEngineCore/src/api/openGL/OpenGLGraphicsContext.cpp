#include "nspch.h"
#include <core/Logger.h>
#include "OpenGLGraphicsContext.h"


namespace TrashEngine {

    bool OpenGLGraphicsContext::init(RenderWindow* window)
    {
        this->m_window = static_cast<GLFWRenderWindow*>(window);

        glfwMakeContextCurrent(this->m_window->getHandle());

        int version = gladLoadGL();
        if (version == 0) {
            NS_CORE_ERROR("Failed to load OpenGL using GLAD");
            return false;
        }

        return true;
    }

    void OpenGLGraphicsContext::cleanUp()
    {
        // TODO cleanUp
    }

}
