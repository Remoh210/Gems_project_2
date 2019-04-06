#include "cAnimationController.h"
#include "cGameObject.h"
#include "globalStuff.h"
#include <iostream>


cAnimationController::cAnimationController(cGameObject* ActiveChar)
{
	mActiveChar = ActiveChar;
}

void cAnimationController::walkForward()
{
	if (mActiveChar->currentAnimation != "Run-jump") {
		mActiveChar->currentAnimation = "Walk-forward";
	}
}

void cAnimationController::JumpForward()
{
}

std::string cAnimationController::GetCurrentAnimation()
{

	glm::vec3 vel = mActiveChar->rigidBody->GetVelocity();
	glm::vec3 pos = mActiveChar->rigidBody->GetPosition();
	//glm::vec3 from;
	float height = 13.0f;
	//from = glm::vec3(pos.x, pos.y -height, pos.z);

	glm::vec3 to = glm::vec3(pos.x, pos.y - height, pos.z);
	g_pDebugRendererACTUAL->addLine(pos, to, glm::vec3(1.0f, 0.0f, 0.0f));

	if (!gPhysicsWorld->RayCast(pos, to))
	{
		return "Run-jump";
	}
	if (abs(vel.x) < 1.01f && abs(vel.z) < 1.01f)
	{
		if (mActiveChar->currentAnimation != "Action2")
		{
			return "Idle";
		}
		else
		{
			this->mActiveChar->currentAnimation;
		}
		
	}
	
	
	return this->mActiveChar->currentAnimation;
	
	
}

void cAnimationController::UpdateController()
{

	glm::vec3 vel = mActiveChar->rigidBody->GetVelocity();
	glm::vec3 pos = mActiveChar->rigidBody->GetPosition();
	if (pos.y > -2.0f)
	{
		this->curState = inAir;
	}
	else if (vel.x < 0.01f, vel.z < 0.01f)
	{
		this->curState = Idle;
	}

}
