//#include "stdafx.h"
#include "shape.h"

namespace nPhysics
{
	cSphereShape::cSphereShape(float radius)
		: iSphereShape()
		, mRadius(radius)
	{

	}
	cSphereShape::cSphereShape()
		: iSphereShape()
	{

	}

	cSphereShape::cSphereShape(const cSphereShape& other)
		: iSphereShape()
	{

	}
	cSphereShape& cSphereShape::operator=(const cSphereShape& other)
	{
		return *this;
	}
	cSphereShape::~cSphereShape()
	{

	}
	bool cSphereShape::GetAABB(const glm::mat4 & transform, glm::vec3 & minBoundsOut, glm::vec3 & maxBoundsOut)
	{
		const glm::vec4& pos = transform[3];
		minBoundsOut.x = pos.x - mRadius;
		minBoundsOut.y = pos.y - mRadius;
		minBoundsOut.z = pos.z - mRadius;

		maxBoundsOut.x = pos.x + mRadius;
		maxBoundsOut.y = pos.y + mRadius;
		maxBoundsOut.z = pos.z + mRadius;
		return true;
	}
	//float cSphereShape::GetRadius()
	//{
	//	return mRadius;
	//}






	cPlaneShape::cPlaneShape(const glm::vec3& normal, float planeConst)
		: iPlaneShape()
		, mNormal(normal)
		, mPlaneConst(planeConst)
	{

	}
	cPlaneShape::cPlaneShape()
		: iPlaneShape()
	{

	}

	cPlaneShape::cPlaneShape(const cPlaneShape& other)
		: iPlaneShape()
	{

	}
	cPlaneShape& cPlaneShape::operator=(const cPlaneShape& other)
	{
		return *this;
	}
	cPlaneShape::~cPlaneShape()
	{

	}

	bool cPlaneShape::GetAABB(const glm::mat4 & transform, glm::vec3 & minBoundsOut, glm::vec3 & maxBoundsOut)
	{
		minBoundsOut.x = std::numeric_limits<float>::min();
		minBoundsOut.y = std::numeric_limits<float>::min();
		minBoundsOut.z = std::numeric_limits<float>::min();

		maxBoundsOut.x = std::numeric_limits<float>::max();
		maxBoundsOut.y = std::numeric_limits<float>::max();
		maxBoundsOut.z = std::numeric_limits<float>::max();
		return true;
	}

	//float cPlaneShape::GetPlaneConstant()
	//{
	//	return this->mPlaneConst;
	//}

	//glm::vec3 cPlaneShape::GetPlaneNormal()
	//{
	//	return this->mNormal;
	//}
	//bool cPlaneShape::getPlaneType(PlaneType& type)
	//{
	//	type = this->type;
	//	return true;
	//}
	//bool cPlaneShape::setGameType(const PlaneType& type)
	//{
	//	this->type = type;
	//	return true;
	//}
	//bool cPlaneShape::GetPlaneNormal(glm::vec3& normalOut)
	//{
	//	normalOut = mNormal;
	//	return true;
	//}
	//bool cPlaneShape::GetPlaneConst(float& planeConstOut)
	//{
	//	planeConstOut = mPlaneConst;
	//	return true;
	//}
}