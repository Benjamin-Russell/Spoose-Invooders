#ifndef SAVESYSTEM_H
#define SAVESYSTEM_H

#include <sys/stat.h>
#include "Trackable.h"
#include <fstream>
#include <iostream>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

using namespace std;

class SaveSystem : public Trackable
{
	const string SAVE_FILE = "Resources/save.dat";
	const bool SAVE_SYSTEM_ENABLED = true;
public:
	SaveSystem();
	~SaveSystem();
	
	bool saveFileExists();
	void saveGame();
	void loadGame();
	void destroySaveFile();
private:
};
#endif