#pragma once
#include <glm.hpp> // This is the main GLM header
#include <gtc/matrix_transform.hpp> // This one lets us use matrix transformations
#include <gtc/type_ptr.hpp> // This one gives us access to a utility function which makes sending data to OpenGL nice and easy
#include <gtc\quaternion.hpp>
#include <iostream>
#include "glew.h"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();
	void GenertateFBO(unsigned int _width, unsigned int _height);
	GLuint GetFBO() { return FBO; }
private:
	GLuint colourTexture;
	GLuint depthTexture;
	GLuint FBO;
	GLenum drawBuffer;
};

