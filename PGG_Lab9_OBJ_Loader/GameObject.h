#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include "Mesh.h"
#include "Material.h"
#include <vector>
#include <iostream>


// The GameObject contains a mesh, a material and position / orientation information
class GameObject
{
public:

	GameObject();
	~GameObject();

	//void SetMesh(Mesh *input) {_mesh = input;}
	//void SetMaterial(Material *input) {_material = input;}

	void SetMesh(std::shared_ptr<Mesh> input) { _mesh = input; }
	void SetMaterial(std::shared_ptr<Material> input) { _material = input; }
	
	void SetPosition( float posX, float posY, float posZ ) {_position.x = posX; _position.y = posY; _position.z = posZ;}

	void SetRotation( float rotX, float rotY, float rotZ ) {_rotation.x = rotX; _rotation.y = rotY; _rotation.z = rotZ;}

	double GetDistanceFromPlayer() { return distanceFromPlayer;  }

	void CalculateDistanceFromPlayer(glm::mat4 _viewMatrix);
	
	// Game object is current hard-coded to rotate
	void Update( float deltaTs, glm::mat4 _viewMatrix);

	// Need to give it the camera's orientation and projection
	void Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix);

protected:

	// The actual model geometry
	//Mesh *_mesh;
	// The material contains the shader
	//Material *_material;

	
	std::shared_ptr<Mesh> _mesh;

	std::shared_ptr<Material> _material;

	// Matrix for the position and orientation of the game object
	glm::mat4 _modelMatrix;
	glm::mat4 _invModelMatrix;
	
	// Position of the model
	// The model matrix must be built from this and the _rotation
	glm::vec3 _position;
	
	// Orientation of the model
	// The model matrix must be built from this and the _position
	glm::vec3 _rotation;

	double distanceFromPlayer;


};



#endif
