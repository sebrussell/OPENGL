
#ifndef __MATERIAL__
#define __MATERIAL__

#include <string>
#include <glm.hpp>
#include <gtc\type_ptr.hpp>
#include "glew.h"
#include "Light.h"
#include <vector>
#include <memory>

// Encapsulates shaders and textures
class Material
{
public:
	Material();
	~Material();

	// Loads shaders from file
	// Returns false if there was an error - it will also print out messages to console
	//bool LoadShaders(std::string vertFilename, std::string fragFilename);
	//bool LoadShaders2(std::string vertFilename, std::string fragFilename);
	bool LoadShaders(std::string vertFilename, std::string fragFilename, int _shaderNumber);

	// For setting the standard matrices needed by the shader
	void SetMatrices(glm::mat4 modelMatrix, glm::mat4 invModelMatrix, glm::mat4 viewMatrix, glm::mat4 projMatrix);

	// For setting material properties
	//void SetEmissiveColour(glm::vec3 input) { _emissiveColour = input; }
	//void SetDiffuseColour(glm::vec3 input) { _diffuseColour = input; }
	//void SetSpecularColour(glm::vec3 input) { _specularColour = input; }

	// Set light position in world space
	//void SetLightPosition(std::vector<Light*> _lights);
	void SetLightPosition(std::vector<std::shared_ptr<Light>> _lights);

	void SetLightAmount(int _amount) { lightAmount = _amount; }
	// Sets texture
	// This applies to ambient, diffuse and specular colours
	// If you want textures for anything else, you'll need to do that yourself ;)
	bool SetTexture( std::string filename ) {_texture1 = LoadTexture(filename); return _texture1>0;}

	// Sets the material, applying the shaders
	void Apply();

	void ShaderPassOne();
	void ShaderPassTwo();

protected:
	
	int lightAmount;

	std::vector<glm::vec4> m_lightPositions;
	std::vector<glm::vec3> m_lightDirection;
	std::vector<glm::vec3> m_lightColour;
	std::vector<glm::vec3> m_lightDiffuseColour;
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

	// Loads a .bmp from file
	unsigned int LoadTexture( std::string filename );
	
	// OpenGL handle for the texture
	unsigned int _texture1;

	



};






#endif
