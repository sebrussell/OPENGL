#include "CubeMap.h"



CubeMap::CubeMap()
{
	m_textureFaces.push_back("purplenebula_bk.bmp");
	m_textureFaces.push_back("purplenebula_bk.bmp");
	m_textureFaces.push_back("purplenebula_bk.bmp");
	m_textureFaces.push_back("purplenebula_bk.bmp");				//these are a list of the 6 different cube maps to use
	m_textureFaces.push_back("purplenebula_bk.bmp");
	m_textureFaces.push_back("purplenebula_bk.bmp");
}


CubeMap::~CubeMap()
{
}

GLuint CubeMap::CreateCubeMap(int _width, int _height)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);		//set up the texture location

	SDL_Surface* sdlSurface;			//make a new sdl surface

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < m_textureFaces.size(); i++)			//for every texture location
	{
		sdlSurface = SDL_LoadBMP(m_textureFaces[i].c_str());		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, sdlSurface->w, sdlSurface->h, 0, GL_BGR, GL_UNSIGNED_BYTE, sdlSurface->pixels);		//iterate through and make the 2d image
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);			//different parameters for the cubemap
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);										

	return textureID;
}