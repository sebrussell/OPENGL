#pragma once
#include <glm.hpp>
#include "glew.h"
#include <vector>					//the include file
#include <string>
#include <SDL.h>
#include <iostream>

class CubeMap
{
public:
	CubeMap();
	~CubeMap();												//the public functions which create the cube map
	GLuint CreateCubeMap(int _width, int _height);
private:
	
	std::vector<std::string> m_textureFaces;				//a vector of the different file locations
};

