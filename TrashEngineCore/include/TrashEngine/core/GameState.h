#pragma once

#include <string>

#include "Time.h"
#include "event/Event.h"

namespace TrashEngine {

	class GameState
	{
	public:
		NS_API GameState(const char* name) : m_name(name) {}
		NS_API GameState(const GameState&) = delete;
		NS_API GameState& operator=(const GameState&) = delete;
		NS_API virtual ~GameState() = default;

		inline NS_API const std::string getName() { return this->m_name; }

		/////////////////////////////////////////////////////////////
		/// Remember to deal with the GLFW event(??¶ç?¶ä??ä¸?è¦????è£¡ä??æ²?å·?)
		/////////////////////////////////////////////////////////////

		NS_API virtual void onEvent(Event& evnt) {};

		NS_API virtual void onUpdate(Time delta) {};

		/////////////////////////////////////////////////////////////
		/// NS engine won't render automatically you must call
		/// GameEngine::Render() to render the process things
		/////////////////////////////////////////////////////////////
		NS_API virtual void onRender(Time time) {};

		NS_API virtual void onInit() {};

		NS_API virtual void onCleanUp() {};

		////////////////////////////////////////////////////////////////
		/// changing state the current state will pause.(will call this)
		////////////////////////////////////////////////////////////////
		NS_API virtual void onPause() {};

		/////////////////////////////////////////////////////////////////
		/// on changing state prepare to change state will call this
		/////////////////////////////////////////////////////////////////
		NS_API virtual void onChange() {};
	private:
		std::string m_name;

	};

}
