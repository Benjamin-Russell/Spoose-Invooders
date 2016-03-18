#include <iostream>
#include <SDL.h>

#include "Game.h"
#include "GraphicsSystem.h"
#include "GraphicsBufferManager.h"

#include <MemoryTracker.h>

// SDL Tutorials http://lazyfoo.net/SDL_tutorials/lesson02/index.php

// A Warning that sometimes pops up: deafultlib 'msvcrt.lib' conflicts with use of other libs; use /NODEFAULTLIB:library

// Since system("pause") is a security concern, can it be replaced with something?

int main(int argc, char** argv)
{
	// Seed rand
	srand(unsigned(time(NULL)));

	// Init SDL
	int toReturn = initSDL();
	if (toReturn != 0)
		return toReturn;

	// Init Game Object
	Game::initInstance();

	// Run Game Loop
	Game::getInstance()->runGameLoop(60);

	// End
	Game::getInstance()->cleanUp();
	delete Game::getInstance();

	gMemoryTracker.reportAllocations(std::cout);

	system("pause");
	return 0;
}