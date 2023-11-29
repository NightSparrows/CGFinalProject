#pragma once

#include "Base.h"
#include "RenderWindow.h"
#include "Keyboard.h"
#include <TrashEngine/graphics/GraphicsContext.h>
#include "GameState.h"


namespace TrashEngine {

	class GameEngine
	{
	public:
		NS_API GameEngine(uint32_t width, uint32_t height, const std::string& title);
		virtual ~GameEngine() = default;

		NS_API bool init();

		NS_API bool run(GameState* state);

		NS_API void stop();

		NS_API void cleanUp();

		NS_API GraphicsContext* getGraphicsContext();

		inline Keyboard* getKeyboard() { return this->m_keyboard.get(); }

	protected:
		Scope<RenderWindow> m_window;
		Scope<Keyboard> m_keyboard;

		Scope<GraphicsContext> m_context;

		GameState* m_state;
		bool m_running;

	};

}
