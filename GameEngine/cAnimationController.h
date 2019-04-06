#pragma once
#include <string>

//forward declaration
class cGameObject;

enum eCharState
{
	Idle = 0,
	inAir = 1

};

class cAnimationController
{
public:
	cAnimationController(cGameObject* ActiveChar);

	std::string GetCurrentAnimation();
	//SetCharacter;

	//Not using yet
	void walkForward();
	void JumpForward();
	void UpdateController();
	

private:

	cGameObject* mActiveChar;
	eCharState curState;
};