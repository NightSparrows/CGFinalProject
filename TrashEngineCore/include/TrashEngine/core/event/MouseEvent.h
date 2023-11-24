#pragma once

#include "Event.h"

namespace TrashEngine {

	class MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(int x, int y) {
			this->x = x;
			this->y = y;
		}

		EVENT_CLASS_TYPE(MouseMoved)

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

		int x, y;
	};

	class MouseButtonPressedEvent : public Event {
	public:
		MouseButtonPressedEvent(int button, bool repeat) {
			this->button = button;
			this->repeat = repeat;
		}

		EVENT_CLASS_TYPE(MouseButtonPressed)

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

		int button;
		bool repeat;
	};

	class MouseButtonReleasedEvent : public Event {
	public:
		MouseButtonReleasedEvent(int button) {
			this->button = button;
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

		int button;
	};

	// Scrolling
	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset) {
			this->xOffset = xOffset;
			this->yOffset = yOffset;
		}

		EVENT_CLASS_TYPE(MouseScrolled);

		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

		float xOffset, yOffset;
	};

}
