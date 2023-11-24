#pragma once

#include <TrashEngine/core/GameState.h>


class FinalProjectState : public TrashEngine::GameState
{
public:
	FinalProjectState(TrashEngine::GameEngine* engine);

	void onEvent(TrashEngine::Event& evnt) override;


private:
	TrashEngine::GameEngine* m_engine;

};

