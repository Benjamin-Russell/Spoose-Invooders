#ifndef AUDIO_SERVICE_H
#define AUDIO_SERVICE_H

#include "Audio.h"
#include <vector>
#include <SDL_mixer.h>

using namespace std;

class AudioService : public Audio
{
public:
	AudioService();
	virtual ~AudioService();
	virtual void playSound(SoundEffect id);
	virtual void stopAllSounds();
private:
	vector<Mix_Chunk*> mSounds;
};
#endif