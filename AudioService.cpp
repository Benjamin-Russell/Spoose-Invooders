#include "AudioService.h"

AudioService::AudioService()
{
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		cout << "Unable to initialize audio system" << endl;
	}

	mSounds.push_back(Mix_LoadWAV("Resources/menu_scroll.wav"));
	mSounds.push_back(Mix_LoadWAV("Resources/menu_select.wav"));
	mSounds.push_back(Mix_LoadWAV("Resources/death.wav"));
	mSounds.push_back(Mix_LoadWAV("Resources/explosion.wav"));
	mSounds.push_back(Mix_LoadWAV("Resources/fire.wav"));
	mSounds.push_back(Mix_LoadWAV("Resources/laser.wav"));
	mSounds.push_back(Mix_LoadWAV("Resources/pickup.wav"));
	mSounds.push_back(Mix_LoadWAV("Resources/launch_explosive.wav"));
	mSounds.push_back(Mix_LoadWAV("Resources/InvaderDead.wav"));
	mSounds.push_back(Mix_LoadWAV("Resources/lifeUp.wav"));
	mSounds.push_back(Mix_LoadWAV("Resources/multi.wav"));
}

AudioService::~AudioService()
{
	for (unsigned i = 0; i < mSounds.size(); i++)
	{
		Mix_FreeChunk(mSounds[i]);
	}

	mSounds.clear();
	Mix_CloseAudio();
}

void AudioService::playSound(SoundEffect id)
{
	Mix_PlayChannel(-1, mSounds[id], 0);
}

void AudioService::stopAllSounds()
{

}
