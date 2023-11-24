#pragma once

#include "Base.h"
#include "RenderWindow.h"
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

	protected:
		Scope<RenderWindow> m_window;
		Scope<GraphicsContext> m_context;

		GameState* m_state;
		bool m_running;

	};

}
