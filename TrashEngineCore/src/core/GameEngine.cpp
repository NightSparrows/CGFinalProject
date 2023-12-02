#include "nspch.h"
#include <core/Logger.h>
#include <TrashEngine/core/GameEngine.h>
#include <api/openGL/OpenGLGraphicsContext.h>
#include "GLFWRenderWindow.h"
#include <TrashEngine/core/Clock.h>

namespace TrashEngine {

	GameEngine::GameEngine(uint32_t width, uint32_t height, const std::string& title) :
		m_running(false)
	{
		this->m_window = CreateScope<GLFWRenderWindow>(width, height, title);
		this->m_context = CreateScope<OpenGLGraphicsContext>();
	}

	bool GameEngine::init()
	{
		// initialize logger
		Logger::Init();
		if (!glfwInit()) {
			NS_CORE_ERROR("Failed to initialize GLFW");
		}

		if (!static_cast<GLFWRenderWindow*>(this->m_window.get())->create()) {
			NS_CORE_ERROR("Failed to create GLFW window.");
			return false;
		}
		this->m_keyboard = CreateScope<Keyboard>(this->m_window.get());
		this->m_mouse = CreateScope<Mouse>(this->m_window.get());

		if (!static_cast<OpenGLGraphicsContext*>(this->m_context.get())->init(this->m_window.get())) {
			NS_CORE_ERROR("Failed to initialize OpenGL context");
			return false;
		}

		return true;
	}

	bool GameEngine::run(GameState* state)
	{
		this->m_state = state;
		this->m_state->onInit();
		static_cast<GLFWRenderWindow*>(this->m_window.get())->setEventCallback([this](Event& event) {
			this->m_state->onEvent(event);
			});
		this->m_running = true;
		Clock clock;
		while (this->m_running) {
			Time delta = clock.restart();
			static_cast<GLFWRenderWindow*>(this->m_window.get())->update();
			this->m_state->onUpdate(delta);
			this->m_state->onRender(delta);
			static_cast<GLFWRenderWindow*>(this->m_window.get())->swapBuffers();
		}
		this->m_state->onCleanUp();

		return true;
	}

	void GameEngine::stop()
	{
		this->m_running = false;
	}

	void GameEngine::cleanUp()
	{
		static_cast<OpenGLGraphicsContext*>(this->m_context.get())->cleanUp();
		static_cast<GLFWRenderWindow*>(this->m_window.get())->destroy();
		glfwTerminate();
	}

	GraphicsContext* GameEngine::getGraphicsContext()
	{
		return this->m_context.get();
	}

}
