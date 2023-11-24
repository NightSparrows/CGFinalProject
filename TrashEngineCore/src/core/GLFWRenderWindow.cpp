
#include "nspch.h"
#include <core/Logger.h>

#include <TrashEngine/core/event/KeyEvent.h>
#include <TrashEngine/core/event/MouseEvent.h>
#include <TrashEngine/core/event/WindowEvent.h>

#include "GLFWRenderWindow.h"

namespace TrashEngine {

	GLFWRenderWindow::GLFWRenderWindow(uint32_t width, uint32_t height, const std::string& title)
	{
		this->m_data.width = width;
		this->m_data.height = height;
		this->m_data.title = title;
		this->m_data.callback = [](Event& event) {};
	}

	GLFWRenderWindow::~GLFWRenderWindow()
	{
		this->destroy();
	}

	bool GLFWRenderWindow::create()
	{
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		this->m_handle = glfwCreateWindow(this->m_data.width, this->m_data.height, this->m_data.title.c_str(), nullptr, nullptr);

		if (!this->m_handle) {
			NS_CORE_ERROR("Failed to create render window.");
			return false;
		}
		glfwSetWindowUserPointer(this->m_handle, &this->m_data);

		glfwSetCursorPosCallback(this->m_handle, [](GLFWwindow* window, double x, double y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			glm::vec2 currentPosition = { (float)x, (float)y };
			data.mouseDeltaPosition = currentPosition - data.lastMousePosition;
			data.lastMousePosition = currentPosition;

			MouseMovedEvent event((int)x, (int)y);
			data.callback(event);
			});

		glfwSetMouseButtonCallback(this->m_handle, [](GLFWwindow* window, int button, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				MouseButtonPressedEvent event(button, false);
				data.callback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				MouseButtonReleasedEvent event(button);
				data.callback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				MouseButtonPressedEvent event(button, true);
				data.callback(event);
				break;
			}
			}
			});

		glfwSetScrollCallback(this->m_handle, [](GLFWwindow* window, double xOffset, double yOffset) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);
			data.callback(event);
			});

		// Window
		glfwSetWindowCloseCallback(this->m_handle, [](GLFWwindow* window) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			/// ?????¶å?????ï¼?ä½?GCCå°±æ?¯æ?????é¡?= =
			WindowCloseEvent* event = new WindowCloseEvent();
			data.callback(*event);
			delete event;
			});

		glfwSetWindowSizeCallback(this->m_handle, [](GLFWwindow* window, int width, int height) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			data.width = width;
			data.height = height;

			WindowResizeEvent event(width, height);
			data.callback(event);
			});

		glfwSetWindowFocusCallback(this->m_handle, [](GLFWwindow* window, int focus) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowFocusEvent event(focus == GLFW_TRUE);
			data.callback(event);
			});

		glfwSetWindowPosCallback(this->m_handle, [](GLFWwindow* window, int x, int y) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowMovedEvent event(x, y);
			data.callback(event);
			});

		// Keyboard
		glfwSetKeyCallback(this->m_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action) {
			case GLFW_PRESS:
			{
				KeyPressedEvent event(key, false);
				data.callback(event);
				break;
			}
			case GLFW_RELEASE:
			{
				KeyReleasedEvent event(key);
				data.callback(event);
				break;
			}
			case GLFW_REPEAT:
			{
				KeyPressedEvent event(key, true);
				data.callback(event);
				break;
			}
			}

			});

		glfwSetCharCallback(this->m_handle, [](GLFWwindow* window, unsigned int keycode) {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			KeyTypedEvent event(keycode);
			data.callback(event);
			});


		return true;
	}

	void GLFWRenderWindow::update()
	{
		this->m_data.mouseDeltaPosition = glm::vec2(0);
		glfwPollEvents();
	}

	void GLFWRenderWindow::swapBuffers()
	{
		glfwSwapBuffers(this->m_handle);
	}

	void GLFWRenderWindow::destroy()
	{
		if (this->m_handle) {
			glfwDestroyWindow(this->m_handle);
			this->m_handle = nullptr;
		}
	}

	void GLFWRenderWindow::setEventCallback(const EventCallbackFn& callback)
	{
		this->m_data.callback = callback;
	}

	uint32_t GLFWRenderWindow::getWidth() const
	{
		return this->m_data.width;
	}

	uint32_t GLFWRenderWindow::getHeight() const
	{
		return this->m_data.height;
	}

}
