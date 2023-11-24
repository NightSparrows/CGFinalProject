
#include <TrashEngine/TrashEngine.h>
#include "FinalProjectState.h"

FinalProjectState::FinalProjectState(TrashEngine::GameEngine* engine) : 
	GameState("finalState"), m_engine(engine)
{
}

void FinalProjectState::onEvent(TrashEngine::Event& evnt)
{
	TrashEngine::EventDispatcher dispatcher(evnt);

	dispatcher.dispatch<TrashEngine::WindowCloseEvent>([this](TrashEngine::WindowCloseEvent& event) {
		this->m_engine->stop();
		return true;
		});
}
