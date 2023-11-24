#pragma once

#include <functional>

#include "../Base.h"


#define NS_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace TrashEngine {

	enum class EventType {
		None = 0,
		WindowClose,
		WindowResize,
		WindowFocus,
		WindowMoved,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

	enum EventCategory {
		None = 0,
		EventCategoryInput = BIT(0),
		EventCategoryWindow = BIT(1),
		EventCategoryMouse = BIT(2),
		EventCategoryKeyboard = BIT(3)
	};

#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
								virtual EventType getEventType() const override { return GetStaticType(); }\
								virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

	class NS_API Event {
		friend class EventDispatcher;
	public:
		/*Event(EventType type, EventCategory category);

		EventType type;
		EventCategory category;*/
		virtual ~Event() = default;

		virtual EventType getEventType() const = 0;
		virtual const char* getName() const = 0;
		virtual int GetCategoryFlags() const = 0;
		virtual std::string toString() const { return getName(); }

		bool isHandled() const;

		void handled();

	protected:
		bool m_Handled = false;

	};

	class NS_API EventDispatcher {
	public:
		EventDispatcher(Event& event) : m_event(event) {

		}

		template<typename T, typename F>
		bool dispatch(const F& func) {
			if (this->m_event.getEventType() == T::GetStaticType()) {
				this->m_event.m_Handled |= func(static_cast<T&>(m_event));
				return true;
			}

			return false;
		}

	private:
		Event& m_event;

	};

}
