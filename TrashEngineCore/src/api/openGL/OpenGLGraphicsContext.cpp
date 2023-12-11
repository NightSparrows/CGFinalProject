#include "nspch.h"
#include <core/Logger.h>

#include <imgui.h>
#include <api/openGL/deps/imgui_impl_glfw.h>
#include <api/openGL/deps/imgui_impl_opengl3.h>

#include "OpenGLGraphicsContext.h"

#include "OpenGLStaticModel.h"
#include "OpenGLMasterRenderer.h"
#include "model/terrain/OpenGLTerrain.h"
#include "model/particle/OpenGLParticle.h"

namespace TrashEngine {
    
    Ref<StaticModel> OpenGLGraphicsContext::createStaticModel()
    {
        return CreateRef<OpenGLStaticModel>();
    }

    Ref<AnimatedModel> OpenGLGraphicsContext::createAnimatedModel()
    {
        return CreateRef<OpenGLAnimatedModel>();
    }

    Ref<Terrain> OpenGLGraphicsContext::createTerrain(float heightIntensity, glm::ivec2 position)
    {
        return CreateRef<OpenGLTerrain>(this->m_normalMapGenerator.get(), heightIntensity, position);
    }

    Ref<Particle> OpenGLGraphicsContext::createParticle(const std::string& filePath, uint32_t numberOfRow)
    {
        return CreateRef<OpenGLParticle>(filePath, numberOfRow);
    }

    Ref<AnimatedModelAnimator> OpenGLGraphicsContext::createAnimatedModelAnimator(Ref<AnimatedModel> animatedModel)
    {
        return CreateRef<OpenGLAnimatedModelAnimator>(StaticCastRef<OpenGLAnimatedModel>(animatedModel));
    }

    Ref<MasterRenderer> OpenGLGraphicsContext::createMasterRenderer(glm::ivec2 renderSize)
    {
        return CreateRef<OpenGLMasterRenderer>(renderSize);
    }

    bool OpenGLGraphicsContext::init(RenderWindow* window)
    {
        this->m_window = static_cast<GLFWRenderWindow*>(window);

        glfwMakeContextCurrent(this->m_window->getHandle());

        int version = gladLoadGL();
        if (version == 0) {
            NS_CORE_ERROR("Failed to load OpenGL using GLAD");
            return false;
        }

#ifdef NS_DEBUG
        GLint flags = 0;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            if (glDebugMessageCallback != nullptr) {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, const void*) {
                    if (id == 131169 ||  // Allocate framebuffer
                        id == 131185 ||  // Allocate buffer
                        id == 131218 ||  // Shader recompile
                        id == 131204 ||  // Texture no base level
                        id == 13         // GL_LIGHTH is deprecated in open GL 3 (deprecated fixed function lights pipleine)
                        )
                        return;
                    NS_CORE_ERROR("ID: {0} Message: {1}\n", id, message);
                    // maybe print more
                    }, nullptr);
                NS_CORE_TRACE("OpenGL debug enable");
            }
            else {
                NS_CORE_WARN("Your system does not support debug layer");
            }
        }

#endif

        // imgui init
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        ImGui::StyleColorsDark();
        ImGui_ImplGlfw_InitForOpenGL(this->m_window->getHandle(), true);
        ImGui_ImplOpenGL3_Init("#version 450");

        // normap map generator init
        this->m_normalMapGenerator = CreateScope<OpenGLNormalMapGenerator>();

        return true;
    }

    void OpenGLGraphicsContext::cleanUp()
    {
        this->m_normalMapGenerator.reset();

        ImGui_ImplOpenGL3_Shutdown();
        // TODO cleanUp
    }

}
