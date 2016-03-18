#ifndef AUDIO_LOCATOR_H
#define AUDIO_LOCATOR_H

#include "AudioService.h"
#include "NullAudioService.h"

using namespace std;

class AudioLocator : public Trackable{
public:
	AudioLocator(){};
	~AudioLocator();

	static Audio* locate(){ return _mService; };
	static void provide(AudioService* srv);
	static void cleanup();
private:
	static Audio* _mService;
	static NullAudioService _mNullService;
};
#endif