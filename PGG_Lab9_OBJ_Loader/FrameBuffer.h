#pragma once
#include <glm.hpp> 
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>						//the include files
#include <gtc\quaternion.hpp>
#include <iostream>
#include "glew.h"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();																//these are used to generate and get the fbo
	void GenertateFBO(unsigned int _width, unsigned int _height);
	GLuint GetFBO() { return FBO; }
private:
	GLuint colourTexture;
	GLuint depthTexture;					//the frame buffer object has these different properties
	GLuint FBO;
	GLenum drawBuffer;
};



