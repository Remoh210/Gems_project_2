#pragma once
#include "physics_interfaces.h"


namespace nPhysics
{
	class cSimpleSoftBody : public iSoftBody
	{
		friend class cSimplePhysicsWorld;
		class cNode;
		class cSpring
		{
		public:
			cSpring(cNode* nodeA, cNode* nodeB, float springConstant);

			void ApplyForceToNodes();

			inline cNode* GetOther(cNode* node)
			{
				return node == NodeA ? NodeB : NodeA;
			}

			cNode* NodeA;
			cNode* NodeB;
			float RestingDistance;
			float SpringConstant;
		};
		class cNode
		{
		public:
			cNode(const glm::vec3& position, float mass);
			inline bool IsFixed()
			{
				return Mass == 0.f;
			}

			bool HasNeighbor(cNode* other);
			void Intgrate(float dt);
			void ComputeRadius();

			glm::vec3 Position;
			glm::vec3 PreviousPosition;
			glm::vec3 Velocity;
			glm::vec3 SpringForce;
			float Radius;
			float Mass;
			std::vector<cSpring*> Springs;
		};


	public:
		cSimpleSoftBody(const sSoftBodyDef& def);
		virtual ~cSimpleSoftBody();

		virtual void GetNodePostion(size_t index, glm::vec3& positionOut);
		virtual void GetNodeRadius(size_t index, float& nodeRadiusOut);
		inline virtual void SwitchWind() { bWind = !bWind; };
		virtual size_t NumNodes();


		virtual void GetAABB(glm::vec3& minBoundsOut, glm::vec3& maxBoundsOut);

		void UpdateInternal(float dt, const glm::vec3& gravity);


	protected:
		cSimpleSoftBody() : iSoftBody() {}
		cSimpleSoftBody(const cSimpleSoftBody& other) : iSoftBody(other) {}
		cSimpleSoftBody& operator=(const cSimpleSoftBody& other) { return *this; }
	private:
		void CollideNodes(cNode* nodeA, cNode* nodeB);
		void ApplyWind(float dt);
		glm::vec3 mWindDirection;
		float mWindMagnitude;
		float Yaw;
		float Pitch;
		bool bWind;

		void UpdateAABB();
		glm::vec3 mMinBounds;
		glm::vec3 mMaxBounds;

		std::vector<cNode*> mNodes;
		std::vector<cSpring*> mSprings;


	};
}

