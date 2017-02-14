#include "Light.h"

Light::Light()
{
	m_position = glm::vec4(0, 10, 0, 0);
	m_lightDirection = glm::vec3(0);
	m_lightColour = glm::vec3(0.1, 0.1, 0.1);
	m_diffuseColour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_ambientColour = glm::vec3(0.1f, 0.1f, 0.2f);
	m_specularColour = glm::vec3(1.0f, 1.0f, 1.0f);
	m_emissiveColour = glm::vec3(0);
	m_lightCutoff = 15;
	m_lightAngle = 0;
	m_lightType = LightType::point;
}

Light::~Light()
{
}
