#include "bullet_shapes.h"
#include "nConvert.h"

namespace nPhysics
{


	cBulletSphereShape::cBulletSphereShape(float radius)
		: iSphereShape()
		, mRadius(radius)
		
	{
		mBulletShape = new btSphereShape(btScalar(mRadius));
	}
	cBulletSphereShape::~cBulletSphereShape()
	{
		delete mBulletShape;
	}




	cBulletPlaneShape::cBulletPlaneShape(const glm::vec3& normal, float planeConst)
		: iPlaneShape()
		, mNormal(normal)
		, mCnonst(planeConst)

	{
		this->mBulletShape = new btStaticPlaneShape(btVector3(normal.x, normal.y, normal.z), planeConst);
	}
	cBulletPlaneShape::~cBulletPlaneShape()
	{
		delete mBulletShape;
	}


	bool cBulletPlaneShape::GetPlaneNormal(glm::vec3& planeNormalOut)
	{
		planeNormalOut = mNormal;
		return true;
	}
	bool cBulletPlaneShape::GetPlaneConstant(float& planeConstantOut)
	{
		planeConstantOut = mCnonst;
		return true;
	}






	cBulletCylinderShape::cBulletCylinderShape(const glm::vec3 & halfExtents, int axis)
		: iCylinderShape()
		, mAxis(axis)
		, mRadius(halfExtents.x)

	{
		if (mAxis == 0)
		{
			//x
			this->mBulletShape = new btCylinderShapeX(nConvert::ToBullet(halfExtents));
		}
		if (mAxis == 1)
		{
			//y
			this->mBulletShape = new btCylinderShape(nConvert::ToBullet(halfExtents));
		}
		else
		{
			//z
			this->mBulletShape = new btCylinderShapeZ(nConvert::ToBullet(halfExtents));

		}
	}

	cBulletCylinderShape::~cBulletCylinderShape()
	{
	}

	float cBulletCylinderShape::GetCylinderRadius()
	{
		if (mAxis == 0)
		{
			//x
			return dynamic_cast<btCylinderShapeX*>(this->mBulletShape)->getRadius();
		}
		if (mAxis == 1)
		{
			//y
			return dynamic_cast<btCylinderShape*>(this->mBulletShape)->getRadius();
		}
		else
		{
			//z
			return dynamic_cast<btCylinderShapeZ*>(this->mBulletShape)->getRadius();

		}
	
	}

	float cBulletCylinderShape::GetCylinderHeight()
	{
		return 0.0f;
	}

	int cBulletCylinderShape::GetCylinderAxis()
	{
		return mAxis;
	}




	cBulletCapsuleShape::cBulletCapsuleShape(float height, float radius, int axis)
		: iCapsuleShape()
		, mRadius(radius)
		, mAxis(axis)
		, mHeight(height)

	{
		{
			if (mAxis == 0)
			{
				//x
				this->mBulletShape = new btCapsuleShapeX(radius, height/2);
			}
			if (mAxis == 1)
			{
				//y
				this->mBulletShape = new btCapsuleShape(radius, height/2);
			}
			else
			{
				//z
				this->mBulletShape = new btCapsuleShapeZ(radius, height/2);
			}
			//mBulletShape = shape;
		}
	}

	cBulletCapsuleShape::~cBulletCapsuleShape()
	{
	}

	float cBulletCapsuleShape::GetCapsuleRadius()
	{
		return this->mRadius;
	}

	float cBulletCapsuleShape::GetCapsuleHeight()
	{
		return this->mHeight;
	}

	int cBulletCapsuleShape::GetCapsuleAxis()
	{
		return this->mAxis;
	}

	cBulletBoxShape::cBulletBoxShape(const glm::vec3 & halfExtents)
	{
		this->mBulletShape = new btBoxShape(nConvert::ToBullet(halfExtents));
	}

	cBulletBoxShape::~cBulletBoxShape()
	{
	}

}
