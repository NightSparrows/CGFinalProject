

#include <TrashEngine/core/GameEngine.h>

#include "FinalProjectState.h"

int main(int argc, char** argv)
{
	TrashEngine::GameEngine engine(1280, 720, "Computer Graphics Final Project");

	engine.init();

	FinalProjectState finalState(&engine);

	engine.run(&finalState);

	engine.cleanUp();

	return 0;
}
