#ifndef _EVENT_SYSTEM_H_
#define _EVENT_SYSTEM_H_

#include "EventListener.h"

#include <Trackable.h>
#include <map>

class EventSystem : public Trackable
{
public:
	EventSystem();
	~EventSystem();

	void fireEvent(const Event& theEvent);
	void addListener(EventType type, EventListener* pListener);

private:
	std::multimap<EventType, EventListener*> mListenerMap;
};

extern EventSystem* gpEventSystem;

#endif