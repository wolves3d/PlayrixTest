#include "stdafx.h"
#include "AudioWrapper.h"

	
bool AudioWrapper::ms_enableFlag = true;


void AudioWrapper::Play(const string & effectID)
{
	if (true == ms_enableFlag)
	{
		MM::manager.PlaySample(effectID);
	}
}