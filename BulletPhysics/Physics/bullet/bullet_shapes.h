#pragma once
#include <Interfaces/iShape.h>
#include <Interfaces/shapes.h>
#include "btBulletCollisionCommon.h"

namespace nPhysics
{
	
	class cBulletSphereShape : public iSphereShape
	{
	public:
		cBulletSphereShape(float radius);
		virtual ~cBulletSphereShape();
		virtual bool GetSphereRadius(float& radiusOut) { radiusOut = this->mRadius; return true; }
		inline btCollisionShape* GetBulletShape()
		{
			return mBulletShape;
		}
	protected:
		float mRadius;
		cBulletSphereShape(const cBulletSphereShape& other) : iSphereShape(other) {}
		cBulletSphereShape& operator=(const cBulletSphereShape& other) { return *this; }
		btCollisionShape* mBulletShape;

	};

	class cBulletPlaneShape :  public iPlaneShape
	{
	public:
		cBulletPlaneShape(const glm::vec3& normal, float planeConst);
		virtual ~cBulletPlaneShape();
		virtual bool GetPlaneNormal(glm::vec3& planeNormalOut);
		virtual bool GetPlaneConstant(float& planeConstantOut);
		inline btCollisionShape* GetBulletShape()
		{
			return mBulletShape;
		}

	protected:
		glm::vec3 mNormal;
		float mCnonst;
		//iShape
		cBulletPlaneShape(const cBulletPlaneShape& other) : iPlaneShape(other) {}
		cBulletPlaneShape& operator=(const cBulletPlaneShape& other) { return *this; }
		btCollisionShape* mBulletShape;

	};

	class cBulletCylinderShape : public iCylinderShape
	{
	public:
		cBulletCylinderShape(const glm::vec3& halfExtents, int axis);
		virtual ~cBulletCylinderShape();
		virtual float GetCylinderRadius();
		virtual float GetCylinderHeight();
		virtual int GetCylinderAxis();
		inline btCollisionShape* GetBulletShape()
		{
			return mBulletShape;
		}

	protected:
		float mRadius;
		int mAxis;
		cBulletCylinderShape(const cBulletCylinderShape& other) : iCylinderShape(other) {}
		cBulletCylinderShape& operator=(const cBulletCylinderShape& other) { return *this; }
		btCollisionShape* mBulletShape;


	};

	class cBulletCapsuleShape : public iCapsuleShape
	{
	public:
		cBulletCapsuleShape(float height, float radius, int axis);
		virtual ~cBulletCapsuleShape();
		virtual float GetCapsuleRadius();
		virtual float GetCapsuleHeight();
		virtual int GetCapsuleAxis();
		inline btCollisionShape* GetBulletShape()
		{
			return mBulletShape;
		}

	protected:
		float mRadius;
		int mAxis;
		float mHeight;
		cBulletCapsuleShape(const cBulletCapsuleShape& other) : iCapsuleShape(other) {}
		cBulletCapsuleShape& operator=(const cBulletCapsuleShape& other) { return *this; }
		btCollisionShape* mBulletShape;


	};

	class cBulletBoxShape : public iBoxShape
	{
	public:
		cBulletBoxShape(const glm::vec3& halfExtents);
		virtual ~cBulletBoxShape();
		inline glm::vec3 GetHalfExtents()
		{
			return mHalfExtents;
		};
		inline btCollisionShape* GetBulletShape()
		{
			return mBulletShape;
		}

	protected:
		glm::vec3 mHalfExtents;
		cBulletBoxShape(const cBulletBoxShape& other) : iBoxShape(other) {}
		cBulletBoxShape& operator=(const cBulletBoxShape& other) { return *this; }
		btCollisionShape* mBulletShape;


	};

}