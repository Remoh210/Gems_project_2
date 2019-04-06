#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "cGameObject.h"

enum Camera_Type
{
	FREE,
	THIRD_PERSON
};


enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{
public:

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	float Yaw;
	float Pitch;
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	bool b_controlledByScript;
	cGameObject* mTarget;
	Camera_Type mCameraType;
	glm::mat4 newViewMat;

	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
		: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
		b_controlledByScript = false;
		//if(target != NULL)
		//{
			mCameraType = THIRD_PERSON;
		//}
		//else
		//{
			mCameraType = FREE;
		//}
		glm::mat4 newViewMat = glm::mat4(0.0f);
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
		b_controlledByScript = false;
		glm::mat4 newViewMat = glm::mat4(0.0f);
	}

	//For Command
	void SetViewMatrix(glm::mat4 newView) { this->newViewMat = newView; }

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		//if (!this->b_controlledByScript) { return glm::lookAt(Position, Position + Front, Up);}
		/*else { return newViewMat; }*/

		switch (mCameraType)
		{
		case FREE:
			return glm::lookAt(Position, Position + Front, Up);
			break;
		case THIRD_PERSON:
			return glm::lookAt(Position, mTarget->position, glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		default:
			break;
		}

	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime;
		if (direction == FORWARD)
			Position += Front * velocity;
		if (direction == BACKWARD)
			Position -= Front * velocity;
		if (direction == LEFT)
			Position -= Right * velocity;
		if (direction == RIGHT)
			Position += Right * velocity;
		if (direction == UP)
			Position += WorldUp * velocity;
		if (direction == DOWN)
			Position -= WorldUp * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}


	void setThirdPerson(cGameObject* target)
	{
		mTarget = target;
		mCameraType = THIRD_PERSON;
	}


	void setFreeCamera()
	{
		Front = glm::vec3(0.0f, 0.0f, -1.0f);
		mCameraType = FREE;
	}


	//void ProcessMouseScroll(float yoffset)
	//{
	//	if (Zoom >= 1.0f && Zoom <= 45.0f)
	//		Zoom -= yoffset;
	//	if (Zoom <= 1.0f)
	//		Zoom = 1.0f;
	//	if (Zoom >= 45.0f)
	//		Zoom = 45.0f;
	//}


	glm::vec3 getDirectionVector()
	{
		glm::vec3 forw = Front - Position;
		forw.y = 0.0f;
		return glm::normalize(forw);
	}


	void updateCameraVectors()
	{
		//glm::vec3 front;
		//front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		//front.y = sin(glm::radians(Pitch));
		//front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		//Front = glm::normalize(front);
		//Right = glm::normalize(glm::cross(Front, WorldUp));  
		//Up = glm::normalize(glm::cross(Right, Front));







		switch (mCameraType)
		{
		case FREE:
		{
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);
			Right = glm::normalize(glm::cross(Front, WorldUp));
			Up = glm::normalize(glm::cross(Right, Front));
		}
		break;
		case THIRD_PERSON:
		{
			// LookAt by GameObject
			Front = mTarget->position;
			//front = m_cameraGO->position;
			//Front += glm::vec3(0.0f, 50.5f, 0.0f);

			// Calculate a direction to guide the new position of the camera
			// related to the center of the Point of Interest
			glm::vec3 posDirection;
			posDirection.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			posDirection.y = sin(glm::radians(-Pitch));
			posDirection.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			posDirection = glm::normalize(posDirection);

			// Project the new position and assign        
			Position = Front + posDirection * 50.0f;
		}
		break;
		default:
			break;
		}



	}

private:
	
	
};


extern glm::vec3 cameraPos;
extern glm::vec3 cameraFront;
extern glm::vec3 cameraUp;
//
//camera control 
extern Camera camera;

#endif