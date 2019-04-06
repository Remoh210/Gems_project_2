#include "cGameObject.h"

//CRITICAL_SECTION CR_MESH_NAME;
cGameObject::cGameObject()
{
	this->position = glm::vec3(0.0f);
	this->nonUniformScale = glm::vec3(1.0f);
	this->setMeshOrientationEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f));
	this->bIsDebug = false;
	this->bIsVisible = true;
	this->bIsWireFrame = false;
	this->bFBO = false;
	this->pAnimController = new cAnimationController(this);
	this->bHadCollision = false;
	this->bIsPlayer = false;
	
	// Set unique ID
	this->m_uniqueID = cGameObject::m_NextID;
	// Increment
	cGameObject::m_NextID++;	// 32 bit - 4 billion

	this->materialDiffuse = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	this->materialSpecular = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	this->bUseVertexColour = false;

	this->bDontLight = false;

	this->velocity = glm::vec3(0.0f);
	this->accel = glm::vec3(0.0f);
	this->bIsUpdatedByPhysics = false;	// physics ignores by default
	this->bIsUpdatedByPhysics = false;
	this->matTransform = glm::mat4(1.0f);

	this->pDebugRenderer = NULL;
	this->pTheShape = NULL;
	this->shapeType = cGameObject::UNKOWN_SHAPE;
	this->rigidBody = NULL;
	this->softBody = NULL;
	//InitializeCriticalSection(&CR_MESH_NAME);


	this->pSimpleSkinnedMesh = NULL;
	this->pAniState = NULL;

	return;
}

void cGameObject::setMeshName(std::string newName)
{

		//EnterCriticalSection(&CR_MESH_NAME);
		this->meshName = newName;
		//LeaveCriticalSection(&CR_MESH_NAME);
	
}

std::string cGameObject::getMeshName()
{
	//EnterCriticalSection(&CR_MESH_NAME);
	return this->meshName;
	//LeaveCriticalSection(&CR_MESH_NAME);
}

void cGameObject::setDiffuseColour(glm::vec3 newDiffuse)
{
	this->materialDiffuse = glm::vec4(newDiffuse, this->materialDiffuse.a);
	return;
}

void cGameObject::setAlphaTransparency(float newAlpha)
{
	this->materialDiffuse.a = newAlpha;
	return;
}

void cGameObject::setSpecularColour(glm::vec3 colourRGB)
{
	this->materialSpecular = glm::vec4(colourRGB, this->materialSpecular.a);
	return;
}

void cGameObject::setSpecularPower(float specPower)
{
	this->materialSpecular.a = specPower;
	return;
}


glm::vec3 cGameObject::getForward(glm::vec3 forwardModelSpace)
{


	glm::vec4 ForwModel = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	glm::quat qOrientation = this->m_meshQOrientation;

	glm::mat4 matOrientation = glm::mat4(qOrientation);

	glm::vec4 vecForwardDirection_WorldSpace = matOrientation * ForwModel;

	// optional normalize
	vecForwardDirection_WorldSpace = glm::normalize(vecForwardDirection_WorldSpace);
	glm::vec3 veccForw(vecForwardDirection_WorldSpace.x, vecForwardDirection_WorldSpace.y, vecForwardDirection_WorldSpace.z);
	return veccForw;

}

void cGameObject::setUniformScale(float scale)
{
	this->nonUniformScale = glm::vec3(scale, scale, scale);
	return;
}



//static 
unsigned int cGameObject::m_NextID = cGameObject::STARTING_ID_VALUE;


void cGameObject::Update(double deltaTime)
{
	if (this->bIsUpdatedByPhysics)
	{
		// Figure out a line showing the velocityz
		glm::vec3 vVel = this->velocity + this->position;
		glm::vec3 vAcc = this->accel + this->position;

		if (this->pDebugRenderer)		// != NULL
		{
			this->pDebugRenderer->addLine(this->position, vVel,
				glm::vec3(0.0f, 1.0f, 0.0f), 0.0f);

			this->pDebugRenderer->addLine(this->position, vAcc,
				glm::vec3(0.0f, 1.0f, 1.0f), 0.0f);
		}//if ( this->pDebugRenderer )	
	}//if ( this->bIsUpdatedByPhysics )

	return;
}
