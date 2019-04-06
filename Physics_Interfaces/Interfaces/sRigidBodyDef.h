#pragma once

#include<glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <string>

namespace nPhysics
{
	struct sRigidBodyDef
	{
		sRigidBodyDef() 
			: Mass(0.0f) 
			, Position(0.0f, 0.0f, 0.0f)
			, Velocity(0.0f, 0.0f, 0.0f)
			, Orientation(0.0f, 0.0f, 0.0f)
			, AngularVelocity(0.0f, 0.0f, 0.0f)
			, GameObjectName("undefined")
			, isPlayer(false)
		{

		}
		float Mass;
		glm::vec3 Position;
		glm::vec3 Velocity;
		bool isPlayer;
		std::string GameObjectName;
		glm::vec3 Orientation; //Euler Angles
		glm::vec3 AngularVelocity;
		glm::quat quatOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	};
}