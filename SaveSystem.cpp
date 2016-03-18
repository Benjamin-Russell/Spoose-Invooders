#include "SaveSystem.h"
#include "Game.h"

SaveSystem::SaveSystem()
{

}

SaveSystem::~SaveSystem()
{

}

// ******************************************Outside Method (Stevie)****************************************
// This function uses the posix function stat() to check and see if a file exists.
// stat() can be two to four times faster than checking with fopen().
// http://www.zentut.com/c-tutorial/c-file-exists/
// http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
// *********************************************************************************************************
bool SaveSystem::saveFileExists()
{
	if (!SAVE_SYSTEM_ENABLED)
		return false;

	struct stat buffer;
	return (stat(SAVE_FILE.c_str(), &buffer) == 0);
}

void SaveSystem::saveGame()
{
	if (!SAVE_SYSTEM_ENABLED)
	{
		cout << "saveGame() failed - SAVE_SYSTEM_ENABLED is false" << endl;
		return;
	}

	if (Game::getInstance()->getScore() == 0 && Game::getInstance()->getPlayer()->getNumLives() == 3)
		return;

	ofstream ofs(SAVE_FILE);
	boost::archive::text_oarchive oa(ofs);
	oa & *Game::getInstance()
		& *Game::getInstance()->getEnemyManager()
		& *Game::getInstance()->getPlayer();
}

void SaveSystem::destroySaveFile()
{
	if (!saveFileExists())
		return;
	
	remove(SAVE_FILE.c_str());
}

void SaveSystem::loadGame()
{
	Game::getInstance()->setLagTime(0.0f);

	if (!SAVE_SYSTEM_ENABLED)
	{
		cout << "loadGame() failed - SAVE_SYSTEM_ENABLED is false" << endl;
		return;
	}

	Game::getInstance()->resetTimer();

	ifstream ifs(SAVE_FILE);
	boost::archive::text_iarchive ia(ifs);
	ia & *Game::getInstance()
		& *Game::getInstance()->getEnemyManager()
		& *Game::getInstance()->getPlayer();

	// Prevent weird deaths on loading
	Game::getInstance()->getPlayer()->becomeInvincible();
}