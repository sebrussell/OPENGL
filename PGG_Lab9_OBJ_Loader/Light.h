#pragma once
#include <glm.hpp>

class Light
{
public:
	Light();
	Light(int x, int y, int z);
	~Light();
	glm::vec4 m_position;
	glm::vec3 m_colour;
};

