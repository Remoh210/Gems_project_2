#pragma once
#include "cGameObject.h"
#include "globalOpenGLStuff.h"
class cSimpleDebugRenderer
{
public:
	cSimpleDebugRenderer(cGameObject* capsule, cGameObject* mUnitSphere, cGameObject* mUnitCube, GLuint IDprogram);

	void drawSphere(glm::vec3 pos, float rad);
	void drawCube(glm::vec3 pos, float side);
	void drawCapsule(glm::vec3 pos);
	

private:
	cGameObject* mUnitSphere;
	cGameObject* mUnitPlane;
	cGameObject* mUnitCube;
	cGameObject* mCapsule;
	GLuint mShaderProgram;
};