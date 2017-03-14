
#include <iostream>
#include <fstream>
#include <SDL.h>
#include <gtc/type_ptr.hpp>						//the include files
#include <gtc/matrix_transform.hpp>
#include "Material.h"


Material::Material()
{
	// Initialise everything here
	_shaderModelMatLocation = 0;
	_shaderViewMatLocation = 0;
	_shaderProjMatLocation = 0;

	_shaderDiffuseColLocation = 0;
	_shaderEmissiveColLocation = 0;					//set some default values
	_shaderSpecularColLocation = 0;

	_shaderTex1SamplerLocation = 0;

	_texture1 = 0;

	lightAmount = 0;

	//windowWidth = _windowWidth;
	//windowHeight = _windowHeight;

	//frameBuffer.GenertateFBO(windowHeight, windowHeight);


}

Material::~Material()
{
	// Clean up everything here
}

bool Material::LoadShaders(std::string vertFilename, std::string fragFilename, int _shaderNumber)
{
	// OpenGL doesn't provide any functions for loading shaders from file


	std::ifstream vertFile(vertFilename);
	char *vShaderText = NULL;

	if (vertFile.is_open())
	{
		// Find out how many characters are in the file
		vertFile.seekg(0, vertFile.end);
		int length = (int)vertFile.tellg();
		vertFile.seekg(0, vertFile.beg);

		// Create our buffer
		vShaderText = new char[length];

		// Transfer data from file to buffer
		vertFile.read(vShaderText, length);

		// Check it reached the end of the file
		if (!vertFile.eof())
		{
			vertFile.close();
			std::cerr << "WARNING: could not read vertex shader from file: " << vertFilename << std::endl;
			return false;
		}

		// Find out how many characters were actually read
		length = (int)vertFile.gcount();

		// Needs to be NULL-terminated
		vShaderText[length - 1] = 0;

		vertFile.close();
	}
	else
	{
		std::cerr << "WARNING: could not open vertex shader from file: " << vertFilename << std::endl;
		return false;
	}


	std::ifstream fragFile(fragFilename);
	char *fShaderText = NULL;

	if (fragFile.is_open())
	{
		// Find out how many characters are in the file
		fragFile.seekg(0, fragFile.end);
		int length = (int)fragFile.tellg();
		fragFile.seekg(0, fragFile.beg);

		// Create our buffer
		fShaderText = new char[length];

		// Transfer data from file to buffer
		fragFile.read(fShaderText, length);

		// Check it reached the end of the file
		if (!fragFile.eof())
		{
			fragFile.close();
			std::cerr << "WARNING: could not read fragment shader from file: " << fragFilename << std::endl;
			return false;
		}

		// Find out how many characters were actually read
		length = (int)fragFile.gcount();

		// Needs to be NULL-terminated
		fShaderText[length - 1] = 0;

		fragFile.close();
	}
	else
	{
		std::cerr << "WARNING: could not open fragment shader from file: " << fragFilename << std::endl;
		return false;
	}

	if (_shaderNumber == 0)
	{
		_shaderProgram = glCreateProgram();
	}
	else if (_shaderNumber == 1)
	{
		_shaderProgram2 = glCreateProgram();				//depening on which shader is being loaded you the shader program will differ
	}
	else if (_shaderNumber == 3)
	{
		_shaderProgram3 = glCreateProgram();
	}

	// The 'program' stores the shaders


	// Create the vertex shader
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	// Give GL the source for it
	glShaderSource(vShader, 1, &vShaderText, NULL);
	// Delete buffer
	delete[] vShaderText;
	// Compile the shader
	glCompileShader(vShader);
	// Check it compiled and give useful output if it didn't work!
	if (!CheckShaderCompiled(vShader))
	{
		std::cerr << "ERROR: failed to compile vertex shader" << std::endl;
		return false;
	}
	// This links the shader to the program

	if (_shaderNumber == 0)
	{
		glAttachShader(_shaderProgram, vShader);
	}
	else if (_shaderNumber == 1)
	{
		glAttachShader(_shaderProgram2, vShader);		//depening on which shader is being loaded you the shader program will differ
	}
	else if (_shaderNumber == 3)
	{
		glAttachShader(_shaderProgram3, vShader);
	}

	// Same for the fragment shader
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fShader, 1, &fShaderText, NULL);
	// Delete buffer
	delete[] fShaderText;
	glCompileShader(fShader);
	if (!CheckShaderCompiled(fShader))
	{
		std::cerr << "ERROR: failed to compile fragment shader" << std::endl;
		return false;
	}

	if (_shaderNumber == 0)
	{
		glAttachShader(_shaderProgram, fShader);
	}
	else if (_shaderNumber == 1)
	{
		glAttachShader(_shaderProgram2, fShader);			//depening on which shader is being loaded you the shader program will differ
	}
	else if (_shaderNumber == 3)
	{
		glAttachShader(_shaderProgram3, fShader);
	}




	if (_shaderNumber == 0)
	{
		// This makes sure the vertex and fragment shaders connect together
		glLinkProgram(_shaderProgram);
		// Check this worked
		GLint linked;
		glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLsizei len;
			glGetProgramiv(_shaderProgram, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetProgramInfoLog(_shaderProgram, len, &len, log);
			std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
			delete[] log;

			return false;
		}
	}
	else if (_shaderNumber == 1)
	{
		// This makes sure the vertex and fragment shaders connect together
		glLinkProgram(_shaderProgram2);
		// Check this worked
		GLint linked;
		glGetProgramiv(_shaderProgram2, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLsizei len;
			glGetProgramiv(_shaderProgram2, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetProgramInfoLog(_shaderProgram2, len, &len, log);
			std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
			delete[] log;

			return false;
		}
	}
	else if (_shaderNumber == 3)
	{
		// This makes sure the vertex and fragment shaders connect together
		glLinkProgram(_shaderProgram3);
		// Check this worked
		GLint linked;
		glGetProgramiv(_shaderProgram3, GL_LINK_STATUS, &linked);
		if (!linked)
		{
			GLsizei len;
			glGetProgramiv(_shaderProgram3, GL_INFO_LOG_LENGTH, &len);

			GLchar* log = new GLchar[len + 1];
			glGetProgramInfoLog(_shaderProgram3, len, &len, log);
			std::cerr << "ERROR: Shader linking failed: " << log << std::endl;
			delete[] log;

			return false;
		}
	}

	//this sets up the individual uniform locations for the shader that is being loaded 


	if (_shaderNumber == 0)
	{
		glAttachShader(_shaderProgram, fShader);

		_shaderModelMatLocation = glGetUniformLocation(_shaderProgram, "modelMat");
		_shaderInvModelMatLocation = glGetUniformLocation(_shaderProgram, "invModelMat");
		_shaderViewMatLocation = glGetUniformLocation(_shaderProgram, "viewMat");
		_shaderProjMatLocation = glGetUniformLocation(_shaderProgram, "projMat");

		_shaderLightAmount = glGetUniformLocation(_shaderProgram, "lightAmount");

		_shaderLightColLocation = glGetUniformLocation(_shaderProgram, "lightColour");
		_shaderEmissiveColLocation = glGetUniformLocation(_shaderProgram, "emissiveColour");
		_shaderAmbientColLocation = glGetUniformLocation(_shaderProgram, "ambientColour");
		_shaderDiffuseColLocation = glGetUniformLocation(_shaderProgram, "diffuseColour");
		_shaderSpecularColLocation = glGetUniformLocation(_shaderProgram, "specularColour");

		_shaderLightPosition = glGetUniformLocation(_shaderProgram, "lightPosition");
		_shaderLightDirection = glGetUniformLocation(_shaderProgram, "lightDirection");
		_shaderLightCutOff = glGetUniformLocation(_shaderProgram, "lightCutOff");
		_shaderLightAngle = glGetUniformLocation(_shaderProgram, "lightAngle");
		_shaderLightType = glGetUniformLocation(_shaderProgram, "lightType");

		_shaderTex1SamplerLocation = glGetUniformLocation(_shaderProgram, "tex1");
	}
	else if (_shaderNumber == 1)
	{
		glAttachShader(_shaderProgram2, fShader);
		_geoShaderTextCoords = glGetUniformLocation(_shaderProgram2, "TexCoords");
	}
	else if (_shaderNumber == 3)
	{
		glAttachShader(_shaderProgram3, fShader);

		_shaderTex1SamplerLocation = glGetUniformLocation(_shaderProgram3, "skybox");
		_shaderViewMatLocation = glGetUniformLocation(_shaderProgram3, "view");
		_shaderProjMatLocation = glGetUniformLocation(_shaderProgram3, "projection");

	}




	return true;
}

bool Material::CheckShaderCompiled(GLint shader)
{
	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);

		// OpenGL will store an error message as a string that we can retrieve and print
		GLchar* log = new GLchar[len + 1];
		glGetShaderInfoLog(shader, len, &len, log);
		std::cerr << "ERROR: Shader compilation failed: " << log << std::endl;
		delete[] log;

		return false;
	}
	return true;
}

unsigned int Material::LoadTexture(std::string filename)
{
	// Load SDL surface
	SDL_Surface *image = SDL_LoadBMP(filename.c_str());

	if (!image) // Check it worked
	{
		std::cerr << "WARNING: could not load BMP image: " << filename << std::endl;
		return 0;
	}

	// Create OpenGL texture
	unsigned int texName = 0;
	glGenTextures(1, &texName);


	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// By default, OpenGL mag filter is linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// By default, OpenGL min filter will use mipmaps
	// We therefore either need to tell it to use linear or generate a mipmap
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// SDL loads images in BGR order
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);

	//glGenerateMipmap(GL_TEXTURE_2D);

	SDL_FreeSurface(image);

	//glBindTexture(GL_TEXTURE_2D, 0);


	return texName;
}

void Material::SetMatrices(glm::mat4 modelMatrix, glm::mat4 invModelMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix)
{
	//glUseProgram(_shaderProgram);
	// Send matrices and uniforms
	glUniformMatrix4fv(_shaderModelMatLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(_shaderInvModelMatLocation, 1, GL_TRUE, glm::value_ptr(invModelMatrix));
	glUniformMatrix4fv(_shaderViewMatLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(_shaderProjMatLocation, 1, GL_FALSE, glm::value_ptr(projMatrix));
	
}

void Material::Apply()
{
	//glUseProgram(_shaderProgram);

	glUniform4fv(_shaderLightPosition, lightAmount, (GLfloat*)&m_lightPositions[0]);
	glUniform3fv(_shaderLightDirection, lightAmount, (GLfloat*)&m_lightDirection[0]);

	glUniform1iv(_shaderLightCutOff, lightAmount, (GLint*)&m_lightCutOffPoint[0]);
	glUniform1fv(_shaderLightAngle, lightAmount, (GLfloat*)&m_lightAngle[0]);
	glUniform1iv(_shaderLightType, lightAmount, (GLint*)&m_lightType[0]);
	glUniform1i(_shaderLightAmount, lightAmount);
																													//apply the different uniform locations
	glUniform3fv(_shaderLightColLocation, lightAmount, (GLfloat*)&m_lightColour[0]);
	glUniform3fv(_shaderEmissiveColLocation, lightAmount, (GLfloat*)&m_lightEmissiveColour[0]);
	glUniform3fv(_shaderAmbientColLocation, lightAmount, (GLfloat*)&m_lightAmbientColour[0]);
	glUniform3fv(_shaderDiffuseColLocation, lightAmount, (GLfloat*)&m_lightDiffuseColour[0]);
	glUniform3fv(_shaderSpecularColLocation, lightAmount, (GLfloat*)&m_lightSpecularColour[0]);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(_shaderTex1SamplerLocation, 0);
	glBindTexture(GL_TEXTURE_2D, _skyBoxTexture);

}

void Material::SetLightPosition(std::vector<std::shared_ptr<Light>> _lights)
{
	lightAmount = _lights.size();
	m_lightPositions.resize(_lights.size());
	m_lightDirection.resize(_lights.size());
	m_lightColour.resize(_lights.size());
	m_lightDiffuseColour.resize(_lights.size());
	m_lightAmbientColour.resize(_lights.size());								//using resize rather than pushback incase this accidently gets called every frame
	m_lightSpecularColour.resize(_lights.size());
	m_lightEmissiveColour.resize(_lights.size());
	m_lightCutOffPoint.resize(_lights.size());
	m_lightAngle.resize(_lights.size());
	m_lightType.resize(_lights.size());

	for (size_t i = 0; i < _lights.size(); i++)
	{
		m_lightPositions.at(i) = _lights[i]->m_position;
		m_lightDirection.at(i) = _lights[i]->m_lightDirection;
		m_lightColour.at(i) = _lights[i]->m_lightColour;
		m_lightDiffuseColour.at(i) = _lights[i]->m_diffuseColour;
		m_lightAmbientColour.at(i) = _lights[i]->m_ambientColour;
		m_lightSpecularColour.at(i) = _lights[i]->m_specularColour;					//update the lighting values
		m_lightEmissiveColour.at(i) = _lights[i]->m_emissiveColour;
		m_lightCutOffPoint.at(i) = _lights[i]->m_lightCutoff;
		m_lightAngle.at(i) = _lights[i]->m_lightAngle;
		m_lightType.at(i) = _lights[i]->m_lightType;
	}
}

void Material::ChangeShader(int _id)
{
	if (_id == 0)
	{
		glUseProgram(_shaderProgram);
	}
	else if (_id == 1)
	{													//alows the program to change between different shaders
		glUseProgram(_shaderProgram2);
	}
	else if (_id == 3)
	{
		glUseProgram(_shaderProgram3);
	}
}

void Material::SetTexture(GLuint _textureColourBuffer)
{
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(_geoShaderTextCoords, 0);
	glBindTexture(GL_TEXTURE_2D, _textureColourBuffer);

}

void Material::SetAsCubeMap(int _width, int _height)
{
	std::vector<std::string> m_textureFaces;
	
	m_textureFaces.push_back("purplenebula_ft.bmp");
	m_textureFaces.push_back("purplenebula_bk.bmp");
	m_textureFaces.push_back("purplenebula_up.bmp");				//information on each file locations
	m_textureFaces.push_back("purplenebula_dn.bmp");
	m_textureFaces.push_back("purplenebula_lf.bmp");
	m_textureFaces.push_back("purplenebula_rt.bmp");										

	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &_skyBoxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _skyBoxTexture);											//some opengl values and paramters ares et
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (size_t i = 0; i < m_textureFaces.size(); i++)
	{																								//for each string in the file locations
		SDL_Surface *image = SDL_LoadBMP(m_textureFaces[i].c_str());
		if (!image) // Check it worked
		{
			std::cerr << "WARNING: could not load BMP image: " << m_textureFaces[i] << std::endl;
		}
		
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, image->w, image->h, 0, GL_BGR, GL_UNSIGNED_BYTE, image->pixels);		//make a texture image 2d
		
	}
}