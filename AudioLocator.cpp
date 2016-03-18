#include "AudioLocator.h"

Audio* AudioLocator::_mService;
NullAudioService AudioLocator::_mNullService;

AudioLocator::~AudioLocator()
{
	cleanup();
}

void AudioLocator::cleanup()
{
	if (_mService != NULL && _mService != &_mNullService)	// Bug fixed! NULL and &_mNullService are not the same
	{
		delete _mService;
	}
}

void AudioLocator::provide(AudioService *srv)
{
	if (srv != NULL)
	{
		_mService = srv;
	}
	else
	{
		if (_mService)
		{
			delete _mService;
			_mService = NULL;
		}
		
		_mService = &_mNullService;
	}
}