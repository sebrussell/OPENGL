
#include "GameObject.h"
#include "Light.h"
#include "FrameBuffer.h"				//include files
#include <string>
#include <memory>
#include <time.h>

class Scene
{
public:	
	Scene(unsigned int _width, unsigned int _height);
	~Scene();

	void ChangeInMousePosition(glm::vec2 _change) { _changeInCameraRotation = _change; }								//public functions
	void ChangeCameraPosition(glm::vec3 _value) { _changeInCameraPosition = _value; }

	void Update( float deltaTs );
	void Draw();

	void FirstPass();					
	void SecondPass();
	void CreateScreenVAO();															//these functions are used for multiple frame buffers
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);

	void Scene::SpawnObject(glm::vec3 changeInPos, glm::vec3 changeInRot, int xPos, int yPos, int zPos);
	void SpawnShooter();																						//these functions are used to spawn items

	void CreateSkyBox();				//this functions is used to make the sky box

protected:
	std::vector<std::shared_ptr<GameObject>> _models;
	GameObject skyBox;

	glm::mat4 _viewMatrix;	
	glm::mat4 _projMatrix;

	std::vector<std::shared_ptr<Light>> _lights;

	glm::vec3 _changeInCameraPosition;											//a list of protected variables
	glm::vec2 _changeInCameraRotation;

	double _viewDistance;
	int _deleteDistance;

	int amountOfLights;

	unsigned int windowWidth, windowHeight;											//this group of variables are used to make multiple frame buffers
	FrameBuffer frameBuffer;
	GameObject drawQuad;
	GLfloat quadVertices[24] = {-1.0f,  1.0f,  0.0f, 1.0f,-1.0f, -1.0f,  0.0f, 0.0f,1.0f, -1.0f,  1.0f, 0.0f,	-1.0f,  1.0f,  0.0f, 1.0f,1.0f, -1.0f,  1.0f, 0.0f,	1.0f,  1.0f,  1.0f, 1.0f};
	GLuint quadVAO, quadVBO;
	GLuint textureColourBuffer;

	std::shared_ptr<Material> modelMaterial;
	std::shared_ptr<Mesh> modelMesh;												//it is probably better for the mesh's and materials to be part of the game object class

	std::shared_ptr<Material> skyboxMaterial;
	std::shared_ptr<Mesh> skyboxMesh;
};
