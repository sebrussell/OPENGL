#pragma once
#include <glm.hpp>				//the include files

class Light
{
public:
	enum LightType
	{
		directional,
		point,							//the diffferent types of light you can have
		spotlight,
		global
	};

	Light();
	~Light();
	glm::vec4 m_position;
	glm::vec3 m_lightColour;
	glm::vec3 m_lightDirection;				//with all the different light properites
	glm::vec3 m_diffuseColour;
	glm::vec3 m_ambientColour;
	glm::vec3 m_specularColour;
	glm::vec3 m_emissiveColour;	
	int m_lightCutoff;
	float m_lightAngle;
	LightType m_lightType;
};

