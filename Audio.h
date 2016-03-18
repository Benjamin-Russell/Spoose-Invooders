#ifndef AUDIO_H
#define AUDIO_H

#include "Trackable.h"

using namespace std;

enum SoundEffect
{
	MENU_SCROLL,
	MENU_SELECT,
	DEATH,
	EXPLOSION,
	FIRE_DEFAULT,
	FIRE_LASER,
	PICKUP,
	FIRE_EXPLOSIVE,
	INVADER_DEAD,
	LIFE_UP,
	MULTI,
	NUM_SOUND_EFFECTS
};

class Audio : public Trackable
{
public:
	virtual ~Audio() {}
	virtual void playSound(SoundEffect id) = 0;
	virtual void stopAllSounds() = 0;
};
#endif