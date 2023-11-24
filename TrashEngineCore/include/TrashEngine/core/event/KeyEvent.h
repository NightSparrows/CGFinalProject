#pragma once

#include "Event.h"

namespace TrashEngine {

	class KeyPressedEvent : public Event {
	public:
		KeyPressedEvent(int key, bool repeat) {
			this->key = key;
			this->repeat = repeat;
		}

		EVENT_CLASS_TYPE(KeyPressed)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

		int key;
		bool repeat;
	};

	class KeyReleasedEvent : public Event {
	public:
		KeyReleasedEvent(int key) {
			this->key = key;
		}

		EVENT_CLASS_TYPE(KeyReleased)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

		int key;
	};

	class KeyTypedEvent : public Event {
	public:
		KeyTypedEvent(int keycode) {
			this->keycode = keycode;
		}

		EVENT_CLASS_TYPE(KeyTyped)
		EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

		int keycode;
	};

}
