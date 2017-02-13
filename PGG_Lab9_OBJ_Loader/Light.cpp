#include "Light.h"

Light::Light()
{
	m_position.x = 0;
	m_position.y = 0;
	m_position.z = 0;
	m_position.w = 1;

	m_colour.x = 0;
	m_colour.y = 0;
	m_colour.z = 0;
}

Light::Light(int x, int y, int z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	m_position.w = 1;

	m_colour.x = 0;
	m_colour.y = 0.05;
	m_colour.z = 0;
}


Light::~Light()
{
}
