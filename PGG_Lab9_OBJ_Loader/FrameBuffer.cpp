#include "FrameBuffer.h"



FrameBuffer::FrameBuffer()
{

}


FrameBuffer::~FrameBuffer()
{
}

void FrameBuffer::GenertateFBO(unsigned int _width, unsigned int _height)
{
	glGenFramebuffers(1, &FBO);                     //Generate a framebuffer object(FBO)
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);         // and bind it to the pipeline

	drawBuffer = GL_COLOR_ATTACHMENT0;
	glDrawBuffers(1, &drawBuffer);

	glGenTextures(1, &colourTexture);
	glBindTexture(GL_TEXTURE_2D, colourTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);												//generate an empty colour texture
	glBindTexture(GL_TEXTURE_2D, 0);

	
	glGenRenderbuffers(1, &depthTexture);
	glBindRenderbuffer(GL_RENDERBUFFER, depthTexture);																//generate an empty render buffer object for depth and stencil
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);


	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colourTexture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthTexture);			//attatch depth/stencil and colour to framebuffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {  //Check for FBO completeness
		std::cout << "Error! FrameBuffer is not complete" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);    //unbind framebuffer
}

