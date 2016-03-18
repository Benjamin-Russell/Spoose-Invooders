#ifndef _GAME_LISTENER_H_
#define _GAME_LISTENER_H_

#include <Trackable.h>

#include "EventSystem.h"

class GameListener : public EventListener
{
public:
	GameListener();
	~GameListener();

	void handleEvent(const Event& theEvent);

private:
	static int msID;
	int mID;

};

#endif