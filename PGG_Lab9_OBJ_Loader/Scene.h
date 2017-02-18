
#include "GameObject.h"
#include "Light.h"
#include "FrameBuffer.h"

// The GLM library contains vector and matrix functions and classes for us to use
// They are designed to easily work with OpenGL!


#include <string>
#include <memory>

// The scene contains objects, the camera and light
// It is responsible for coordinating these things
class Scene
{
public:

	
	// Currently the scene is set up in the constructor
	// This means the object(s) are loaded, given materials and positions as well as the camera and light
	Scene(unsigned int _width, unsigned int _height);
	~Scene();

	// Use these to adjust the camera's orientation
	// Camera is currently set up to rotate about the world-space origin NOT the camera's origin
	void ChangeInMousePosition(glm::vec2 _change) { _changeInCameraRotation = _change; }

	void ChangeCameraPosition(glm::vec3 _value) { _changeInCameraPosition = _value; }

	// Calls update on all objects in the scene
	void Update( float deltaTs );

	// Draws the scene from the camera's point of view
	void Draw();

	void FirstPass();
	void SecondPass();

	void CreateScreenVAO();
	GLuint generateAttachmentTexture(GLboolean depth, GLboolean stencil);

	void Scene::SpawnObject(glm::vec3 changeInPos, glm::vec3 changeInRot, int xPos, int yPos, int zPos);
	void SpawnShooter();

protected:

	// Currently one object, this could be a list of objects!
	//GameObject *_model;
		
	//std::vector<std::vector<GameObject*>> _models;
	std::vector<std::shared_ptr<GameObject>> _models;


	// This matrix represents the camera's position and orientation
	glm::mat4 _viewMatrix;
	
	// This matrix is like the camera's lens
	glm::mat4 _projMatrix;

	// Position of the single point-light in the scene
	//glm::vec3 _lightPosition;
	//std::vector<Light*> _lights;
	std::vector<std::shared_ptr<Light>> _lights;


	glm::vec3 _changeInCameraPosition;
	glm::vec2 _changeInCameraRotation;

	double _viewDistance;

	int _deleteDistance;

	int amountOfLights;

	unsigned int windowWidth, windowHeight;

	FrameBuffer frameBuffer;

	GameObject drawQuad;
	GLfloat quadVertices[24] = {-1.0f,  1.0f,  0.0f, 1.0f,-1.0f, -1.0f,  0.0f, 0.0f,1.0f, -1.0f,  1.0f, 0.0f,	-1.0f,  1.0f,  0.0f, 1.0f,1.0f, -1.0f,  1.0f, 0.0f,	1.0f,  1.0f,  1.0f, 1.0f};

	GLuint quadVAO, quadVBO;
	GLuint textureColourBuffer;

	std::shared_ptr<Material> modelMaterial;
	std::shared_ptr<Mesh> modelMesh;
};
