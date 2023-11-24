#pragma once

#include "Event.h"

namespace TrashEngine {

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() { }

		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)

	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int width, int height) {
			this->width = width;
			this->height = height;
		}

		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)

		int width, height;
	};

	class WindowFocusEvent : public Event {
	public:
		WindowFocusEvent(bool focus) : m_focus(focus) { }

		EVENT_CLASS_TYPE(WindowFocus)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)

		bool isFocus() const { return this->m_focus; }

	private:
		bool m_focus;
	};

	class WindowMovedEvent : public Event {
	public:
		WindowMovedEvent(int x, int y) {
			this->x = x;
			this->y = y;
		}

		EVENT_CLASS_TYPE(WindowMoved)
		EVENT_CLASS_CATEGORY(EventCategoryWindow)

		int x, y;
	};

}
