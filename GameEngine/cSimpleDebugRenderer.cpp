#include "cSimpleDebugRenderer.h"
#include "globalStuff.h"


cSimpleDebugRenderer::cSimpleDebugRenderer(cGameObject* capsule, cGameObject* mUnitSphere, cGameObject* mUnitCube, GLuint IDprogram)
	:mUnitSphere(mUnitSphere),
	mUnitCube(mUnitCube),
	mShaderProgram(IDprogram),
	mCapsule(capsule)
{

}

void cSimpleDebugRenderer::drawSphere(glm::vec3 pos, float rad)
{
	glm::mat4 iden(1.0f);
	mUnitSphere->position = pos;
	mUnitSphere->setDiffuseColour(glm::vec3(0.0f, 1.0f, 0.0f));
	mUnitSphere->setUniformScale(rad);
	mUnitSphere->bIsVisible = true;
	mUnitSphere->bDontLight = true;
	DrawObject(mUnitSphere, iden, mShaderProgram);
	mUnitSphere->bIsVisible = false;
}

void cSimpleDebugRenderer::drawCube(glm::vec3 pos, float side)
{
	glm::mat4 iden(1.0f);
	mUnitCube->position = pos;
	mUnitCube->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
	mUnitCube->setUniformScale(side);
	mUnitCube->bIsVisible = true;
	mUnitCube->bDontLight = true;
	DrawObject(mUnitCube, iden, mShaderProgram);
	mUnitCube->bIsVisible = false;
}

void cSimpleDebugRenderer::drawCapsule(glm::vec3 pos)
{
	glm::mat4 iden(1.0f);
	mCapsule->position = pos;
	mCapsule->setDiffuseColour(glm::vec3(1.0f, 0.0f, 0.0f));
	mCapsule->bIsVisible = true;
	mCapsule->bDontLight = true;
	mUnitSphere->setUniformScale(20.0f);
	DrawObject(mUnitCube, iden, mShaderProgram);
	mCapsule->bIsVisible = false;
}
