#pragma once

#include <TrashEngine/core/GameState.h>


class FinalProjectState : public TrashEngine::GameState
{
public:
	FinalProjectState(TrashEngine::GameEngine* engine);

	void onEvent(TrashEngine::Event& evnt) override;

	void onInit() override;

	void onUpdate(TrashEngine::Time delta) override;

	void onRender(TrashEngine::Time time) override;

	void onCleanUp() override;

private:
	TrashEngine::GameEngine* m_engine;

};

