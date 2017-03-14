#ifndef __MATERIAL__
#define __MATERIAL__

#include <string>
#include <glm.hpp>
#include <gtc\type_ptr.hpp>
#include "glew.h"
#include "Light.h"						//the include files
#include <vector>
#include <memory>
#include "CubeMap.h"

class Material
{
public:
	Material();
	~Material();

	bool LoadShaders(std::string vertFilename, std::string fragFilename, int _shaderNumber);		//you can also load multiple shaders at once (for the multiple frame buffers)

	void SetMatrices(glm::mat4 modelMatrix, glm::mat4 invModelMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix);

	void SetLightPosition(std::vector<std::shared_ptr<Light>> _lights);							//calling this sets up the lights (not just the position)

	void SetLightAmount(int _amount) { lightAmount = _amount; }

	bool SetTexture( std::string filename ) {_texture1 = LoadTexture(filename); return _texture1>0;}		//set textue

	void Apply();

	void ChangeShader(int _id);

	void SetTexture(GLuint _textureColourBuffer);

	unsigned int LoadTexture(std::string filename);
	
	void SetAsCubeMap(int _width, int _heigh);																//creates the cube map for the sky box

protected:

	//the protectd variables are listed below

	int lightAmount;

	std::vector<glm::vec4> m_lightPositions;
	std::vector<glm::vec3> m_lightDirection;
	std::vector<glm::vec3> m_lightColour;
	std::vector<glm::vec3> m_lightDiffuseColour;				//have to have a vector of each different light propety because you cant pass in a class
	std::vector<glm::vec3> m_lightAmbientColour;
	std::vector<glm::vec3> m_lightSpecularColour;
	std::vector<glm::vec3> m_lightEmissiveColour;
	std::vector<int> m_lightCutOffPoint;
	std::vector<float> m_lightAngle;
	std::vector<int> m_lightType;
	

	// Utility function
	bool CheckShaderCompiled( GLint shader );
	
	// The OpenGL shader program handle
	int _shaderProgram;
	int _shaderProgram2;
	int _shaderProgram3;

	// Locations of Uniforms in the vertex shader
	int _shaderModelMatLocation;
	int _shaderInvModelMatLocation;
	int _shaderViewMatLocation;
	int _shaderProjMatLocation;
	// Location of Uniforms in the fragment shader
	int _shaderDiffuseColLocation, _shaderEmissiveColLocation, _shaderSpecularColLocation, _shaderAmbientColLocation, _shaderLightColLocation;
	int _shaderTex1SamplerLocation;
	int _shaderLightPosition, _shaderLightDirection, _shaderLightCutOff, _shaderLightAngle, _shaderLightType;
	int _shaderLightAmount;


	//locations of things in the geomatry shaders

	int _geoShaderTextCoords;



	// Loads a .bmp from file
	CubeMap cubeMap;
	
	// OpenGL handle for the texture
	unsigned int _texture1;
	unsigned int _skyBoxTexture;

	unsigned int _depthTexture;
	unsigned int _colorTexture;

	//unsigned int windowWidth, windowHeight;

	//FrameBuffer frameBuffer;
};






#endif
