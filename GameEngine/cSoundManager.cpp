#include "cSoundManager.h"
#include <fstream>
#include <iostream>

cSoundManager::cSoundManager()
{
}

bool cSoundManager::InitFmod()
{
	//Create fmod system
	this->mResult = FMOD::System_Create(&this->mSystem);
	if (this->mResult != FMOD_OK)
	{
		fprintf(stderr, "FMOD error! (%d) %s\n", this->mResult, FMOD_ErrorString(this->mResult));
		system("pause");
		exit(-1);
	}
	//Initialize fmod system
	this->mResult = mSystem->init(512, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
	if (this->mResult != FMOD_OK)
	{
		fprintf(stderr, "FMOD error! (%d) %s\n", this->mResult, FMOD_ErrorString(this->mResult));
		system("pause");
		exit(-1);
	}
	fprintf(stderr, "FMOD Init OK!\n");
	return true;
}
