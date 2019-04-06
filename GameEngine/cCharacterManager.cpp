#include "cCharacterManager.h"
#include <iostream>

cCharacterManager::cCharacterManager(std::vector<cGameObject*> vecGO)
{
	for (int i = 0; i < vecGO.size(); i++)
	{
		if (vecGO[i]->bIsPlayer)
		{
			mMapCharacters[vecGO[i]->friendlyName] = vecGO[i];
		}
		
	}
}

cGameObject* cCharacterManager::getActiveChar()
{
	return mActiveCharacter;
}

void cCharacterManager::setActiveChar(cGameObject * newActiveChar)
{
	mActiveCharacter = newActiveChar;
}

void cCharacterManager::setActiveChar(std::string GoName)
{
	std::map<std::string, cGameObject*>::iterator it = mMapCharacters.find(GoName);

	if (it == mMapCharacters.end())
	{
		std::cout << GoName << " character not found" << std::endl;
		return;
	}
	else 
	{
		std::cout << GoName << " character was selected" << std::endl;
		mActiveCharacter = it->second; 
	}
	return;
}

void cCharacterManager::setAllChatVel()
{
	std::map<std::string, cGameObject*>::iterator it;

	for(it = mMapCharacters.begin(); it != mMapCharacters.end(); it++)
	{
		if ((*it).second != mActiveCharacter)
		{
			glm::vec3 vel = (*it).second->rigidBody->GetVelocity();
			vel.x = 0.0f;
			vel.z = 0.0f;
			(*it).second->rigidBody->SetVelocity(vel);
		}
		
	}
	//it++
}
