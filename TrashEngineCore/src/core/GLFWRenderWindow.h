#pragma once

#include <TrashEngine/core/RenderWindow.h>
#include <TrashEngine/deps/glm/glm.hpp>

#include <TrashEngine/core/event/Event.h>

#include <GLFW/glfw3.h>

namespace TrashEngine {

	class GLFWRenderWindow : public RenderWindow
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		GLFWRenderWindow(uint32_t width, uint32_t height, const std::string& title);
		~GLFWRenderWindow();

		bool create();

		void update();

		// for openGL
		void swapBuffers();

		void destroy();

		void setEventCallback(const EventCallbackFn& callback);

		uint32_t getWidth() const override;

		uint32_t getHeight() const override;

		struct WindowData
		{
			uint32_t width, height;
			std::string title;
			glm::vec2 mouseDeltaPosition;
			glm::vec2 lastMousePosition;
			EventCallbackFn callback;
		};

		WindowData* getWindowData() { return &this->m_data; }

		inline GLFWwindow* getHandle() { return this->m_handle; }

	private:
		GLFWwindow* m_handle;

		WindowData m_data;

	};

}
