#ifndef _EVENT_H_
#define _EVENT_H_

#include <trackable.h>

// KEY_UP events are wonky and trigger on KEY_DOWN, so let's avoid using them
enum EventType
{
	INVALID = -1,

	// Input events
	LEFT_PRESSED,
	LEFT_HELD,
	LEFT_RELEASED,
	RIGHT_PRESSED,
	RIGHT_HELD,
	RIGHT_RELEASED,
	UP_PRESSED,
	UP_HELD,
	DOWN_PRESSED,
	DOWN_HELD,
	ESCAPE_PRESSED,
	ENTER_PRESSED,
	SPACE_HELD,
	B_PRESSED,
	NUM_1_PRESSED,
	NUM_2_PRESSED,
	NUM_3_PRESSED,
	NUM_4_PRESSED,

	// Game events
	INVADER_HIT_WALL,
	INVADER_HIT_PLAYER,
	PLAYERS_DEAD, // Leaving room for 2 player

	NUM_EVENT_TYPES
};

class Event : public Trackable
{
public:
	Event(EventType type);
	virtual ~Event();

	inline EventType getType() const { return mType; };

private:
	EventType mType;
};

#endif