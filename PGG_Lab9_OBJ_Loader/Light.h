#pragma once
#include <glm.hpp>

class Light
{
public:
	Light(int x, int y, int z);
	~Light();
	glm::vec3 m_position;
};

