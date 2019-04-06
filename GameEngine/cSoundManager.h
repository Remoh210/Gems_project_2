#pragma once
#include <glm\vec3.hpp>
#include <fmod\fmod.hpp>
#include <fmod\fmod_errors.h>
#include <vector>

class cSoundManager 
{
public:
	cSoundManager();
	bool InitFmod();
	//void ProcessAllSounds();


private:
	FMOD_RESULT mResult;
	FMOD::System* mSystem;
	std::vector<FMOD::Sound*> mSounds;
	std::vector<FMOD::Channel*> mChannels;

	// Channel groups
	FMOD::ChannelGroup* mGroups[5];
	FMOD::ChannelGroup* mMasterGroup;

};