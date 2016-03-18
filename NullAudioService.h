#ifndef NULL_AUDIO_SERVICE_H
#define NULL_AUDIO_SERVICE_H

#include "Audio.h"
#include <vector>
#include <SDL_mixer.h>

using namespace std;

class NullAudioService : public Audio
{
public:
	NullAudioService() {};
	virtual ~NullAudioService() {};
	virtual void playSound(SoundEffect id) {};
	virtual void stopAllSounds() {};
};
#endif