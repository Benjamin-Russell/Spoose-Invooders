#include "GameListener.h"
#include "Game.h"

int GameListener::msID = 0;

GameListener::GameListener()
{
	mID = msID;
	msID++;

	// Add this listener to each game event in the Event System
	gpEventSystem->addListener(INVADER_HIT_WALL, this);
	gpEventSystem->addListener(INVADER_HIT_PLAYER, this);
	gpEventSystem->addListener(PLAYERS_DEAD, this);
}

GameListener::~GameListener()
{

}

void GameListener::handleEvent(const Event &theEvent)
{
	switch (theEvent.getType())
	{
	case INVADER_HIT_WALL:
		Game::getInstance()->getEnemyManager()->setMustSwitchDirections(true);
		break;
	case INVADER_HIT_PLAYER:
		Game::getInstance()->getEnemyManager()->resetPositions();
		break;
	case PLAYERS_DEAD:
		Game::getInstance()->getEnemyManager()->resetPositions();
		break;
	}
}