#pragma once

#include "globalOpenGLStuff.h"
#include "cGameObject.h"
#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <vector>
#include <string>

struct sLight
{

	sLight();

	enum eLightType
	{					
		DIRECTIONAL_LIGHT,
		POINT_LIGHT,		
		SPOT_LIGHT			
	};

	glm::vec4 position;
	glm::vec4 diffuse;
	glm::vec4 specular;	
	glm::vec4 atten;		
	glm::vec4 direction;	
	glm::vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	glm::vec4 param2;	// x = 0 for off, 1 for on


	std::string lightName;
	void SetLinearAttenuation(float newLinAtten);
	void SetPosition(glm::vec3 newPosition);
	void SetSpecularHighlight(glm::vec3 rgbColour, float power);

	int position_UniLoc;
	int diffuse_UniLoc;
	int specular_UniLoc;
	int atten_UniLoc;
	int direction_UniLoc;
	int param1_UniLoc;
	int param2_UniLoc;

	void SetLightType(std::string typeAsString);
	void SetLightType(eLightType lightType);
	sLight::eLightType GetLightType_enum(void);
	std::string GetLightType_str(void);
	void SetSpotConeAngles(float innerAngleDegrees, float outerAngleDegrees);
	void SetRelativeDirection(glm::vec3 relDirection);
	void SetRelativeDirectionByEulerAngles(glm::vec3 directionAngle);
	void SetRelativeDirectionByLookAt(glm::vec3 pointInWorldXYZ);
	void SetRelativeDirectionByLookAt(cGameObject* lookAtObj);
	cGameObject* ObjectRelativeTo;
	cGameObject* ObjectLookAt;


	bool AtenSphere;

private:
	eLightType m_lightType;

};

class cLightManager
{
public:
	cLightManager();

	void TurnOnLight(int index);
	void ChangeLightType(std::string lightType, int index);
	std::vector<sLight*> vecLights;
	void LoadUniformLocations(int shaderID);
	void CopyLightValuesToShader(void);

};


