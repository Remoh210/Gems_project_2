#include "cSimpleSoftBody.h"
#include <algorithm>
namespace nPhysics

{
	float elapsedTime = 0.f;

	//Random float generator
	float RandomFloat(float a, float b) {
		float random = ((float)rand()) / (float)RAND_MAX;
		float diff = b - a;
		float r = random * diff;
		return a + r;
	}

	cSimpleSoftBody::cSimpleSoftBody(const sSoftBodyDef& def)
	{
		bWind = false;
		mWindDirection = glm::vec3(0.0f, 0.0f, 1.0f);
		mWindMagnitude = 3.0f;

		mNodes.resize(def.Nodes.size());
		for (size_t i = 0; i < def.Nodes.size(); i++)
		{
			mNodes[i] = new cNode(def.Nodes[i].Position, def.Nodes[i].Mass);
		}

		for (size_t i = 0; i < def.Springs.size(); i++)
		{
			cNode* nodeA = mNodes[def.Springs[i].first];
			cNode* nodeB = mNodes[def.Springs[i].second];
			if (!nodeA->HasNeighbor(nodeB))
			{
				cSpring* spring = new cSpring(nodeA, nodeB, def.SpringConstant);
				mSprings.push_back(spring);
				nodeA->Springs.push_back(spring);
				nodeB->Springs.push_back(spring);
			}
			for (size_t i = 0; i < def.Nodes.size(); i++)
			{
				mNodes[i]->ComputeRadius();
			}

		}
	}

	cSimpleSoftBody::~cSimpleSoftBody()
	{
		for (size_t i = 0; i < mNodes.size(); i++)
		{
			delete mNodes[i];
		}
		mNodes.clear();
		for (size_t i = 0; i < mSprings.size(); i++)
		{
			delete mSprings[i];
		}
		mSprings.clear();
	}

	void cSimpleSoftBody::GetNodePostion(size_t index, glm::vec3 & positionOut)
	{
		positionOut = mNodes[index]->Position;
	}

	void cSimpleSoftBody::GetNodeRadius(size_t index, float & nodeRadiusOut)
	{
		nodeRadiusOut = mNodes[index]->Radius;
	}

	size_t cSimpleSoftBody::NumNodes()
	{
		return mNodes.size();
	}

	void cSimpleSoftBody::GetAABB(glm::vec3 & minBoundsOut, glm::vec3 & maxBoundsOut)
	{
		minBoundsOut = mMinBounds;
		maxBoundsOut = mMaxBounds;
	}

	void cSimpleSoftBody::UpdateInternal(float dt, const glm::vec3& gravity)
	{
		//Applying wind first
		ApplyWind(dt);

		for (size_t i = 0; i < mNodes.size(); i++)
		{
			mNodes[i]->SpringForce = gravity * mNodes[i]->Mass;
		}
		for (size_t i = 0; i < mSprings.size(); i++)
		{
			mSprings[i]->ApplyForceToNodes();

		}
		for (size_t i = 0; i < mNodes.size(); i++)
		{
			mNodes[i]->Intgrate(dt);
		}
		for (size_t idxA = 0; idxA < mNodes.size(); idxA++)
		{
			for (size_t idxB = 0; idxB < mNodes.size(); idxB++)
			{
				CollideNodes(mNodes[idxA], mNodes[idxB]);
			}
		}

		UpdateAABB();

	}



	void cSimpleSoftBody::ApplyWind(float dt)
	{
		//Randomizing Wind
		if (bWind) {
			elapsedTime += dt;
			if (elapsedTime > 3.0f)
			{
				mWindDirection = glm::vec3(RandomFloat(-0.2f, 0.2f), RandomFloat(-0.2f, 0.2f), 1.0f);
				mWindMagnitude = RandomFloat(-1.0f, 3.0f);
				elapsedTime = 0.0f;
			}

			for (int i = 0; i < mNodes.size(); i++)
			{
				if (!mNodes[i]->IsFixed()) {
					float r = RandomFloat(0.0f, 1.0f);
					if (r > 0.5f) {
						mNodes[i]->Velocity += mWindDirection * (mWindMagnitude);
					}
				}
			}
		}
	}

	void cSimpleSoftBody::CollideNodes(cNode * nodeA, cNode * nodeB)
	{
		if (nodeA->HasNeighbor(nodeB))
		{
			if (nodeA != nodeB) {
				if (!nodeA->IsFixed() && !nodeB->IsFixed())
				{


					glm::vec3 diffAB = glm::normalize(nodeA->Position - nodeB->Position);
					glm::vec3 differenceB = glm::normalize(nodeB->Position - nodeA->Position);
					float dist = glm::distance(nodeA->Position, nodeB->Position);

					if (dist <= (nodeA->Radius + nodeB->Radius))
					{
						float totalMass = nodeA->Mass + nodeB->Mass;
						float massAPercent = nodeA->Mass / totalMass;
						float massBPercent = nodeB->Mass / totalMass;

						float aDot = glm::dot(diffAB, nodeA->Velocity);
						glm::vec3 v0 = diffAB * aDot;
						glm::vec3 v1 = nodeA->Velocity - v0;

						diffAB *= -1.0f;
						float bDot = glm::dot(diffAB, nodeB->Velocity);
						glm::vec3 v2 = diffAB * bDot;
						glm::vec3 v3 = nodeB->Velocity - v2;

						nodeA->Velocity = -1.0f * (v0 * (nodeA->Mass - nodeB->Mass) / (nodeA->Mass + nodeB->Mass) + v2 * (2 * nodeB->Mass) / (nodeA->Mass + nodeB->Mass) + v1);
						nodeB->Velocity = -1.0f * (v0 * (2 * nodeA->Mass) / (nodeA->Mass + nodeB->Mass) + v2 * (nodeB->Mass - nodeA->Mass) / (nodeA->Mass + nodeB->Mass) + v3);

						nodeA->Velocity *= 0.99f;
						nodeB->Velocity *= 0.99f;


					}
				}
			}
		}

	}



	void cSimpleSoftBody::UpdateAABB()
	{
		mMinBounds.x = std::numeric_limits<float>::max();
		mMinBounds.y = std::numeric_limits<float>::max();
		mMinBounds.z = std::numeric_limits<float>::max();

		mMaxBounds.x = std::numeric_limits<float>::min();
		mMaxBounds.y = std::numeric_limits<float>::min();
		mMaxBounds.z = std::numeric_limits<float>::min();

		for (size_t i = 0; i < mNodes.size(); i++)
		{
			cNode* node = mNodes[i];

			mMinBounds.x = glm::min(mMinBounds.x, node->Position.x - node->Radius);
			mMinBounds.y = glm::min(mMinBounds.y, node->Position.y - node->Radius);
			mMinBounds.z = glm::min(mMinBounds.z, node->Position.z - node->Radius);

			mMaxBounds.x = glm::max(mMaxBounds.x, node->Position.x + node->Radius);
			mMaxBounds.y = glm::max(mMaxBounds.y, node->Position.y + node->Radius);
			mMaxBounds.z = glm::max(mMaxBounds.z, node->Position.z + node->Radius);

		}


	}

	cSimpleSoftBody::cSpring::cSpring(cNode * nodeA, cNode * nodeB, float springConstant)
		: NodeA(nodeA)
		, NodeB(nodeB)
		, SpringConstant(springConstant)
		, RestingDistance(glm::length(NodeA->Position - NodeB->Position))
	{
	}

	void cSimpleSoftBody::cSpring::ApplyForceToNodes()
	{
		//Hook’s Law
		glm::vec3 separationAtoB = NodeB->Position - NodeA->Position;
		float CurrentDistance = glm::length(separationAtoB);
		glm::vec3 force = (separationAtoB / CurrentDistance) * (-SpringConstant * (CurrentDistance - RestingDistance));
		NodeB->SpringForce += force;
		NodeA->SpringForce -= force;

	}

	//Node constructor
	cSimpleSoftBody::cNode::cNode(const glm::vec3 & position, float mass)
		: Position(position)
		, PreviousPosition(position)
		, Velocity(0.f, 0.f, 0.f)
		, SpringForce(0.f, 0.f, 0.f)
		, Mass(mass)
	{
	}

	bool cSimpleSoftBody::cNode::HasNeighbor(cNode * other)
	{
		return Springs.end() != std::find_if(Springs.begin(), Springs.end(), [other, this](cSpring* spring) {
			return other == spring->GetOther(this);
		});

	}

	void cSimpleSoftBody::cNode::Intgrate(float dt)
	{

		if (IsFixed())
		{
			return;
		}
		PreviousPosition = Position;
		Velocity += SpringForce * (dt / Mass);
		Position += Velocity * dt;
		Velocity *= 0.99f;
		float dampingFactor = 0.9f;
		Velocity *= glm::pow(1.f - dampingFactor, dt);
	}
	void cSimpleSoftBody::cNode::ComputeRadius()
	{
		float closestNeighbor = std::numeric_limits<float>::max();
		for (size_t i = 0; i < Springs.size(); i++)
		{
			float distance = glm::length(Springs[i]->GetOther(this)->Position - Position);
			closestNeighbor = glm::min(closestNeighbor, distance);
		}

		Radius = 0.5f * closestNeighbor;
	}
}

