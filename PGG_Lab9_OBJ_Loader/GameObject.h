#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include "Mesh.h"
#include "Material.h"
#include <vector>									//the include files
#include <iostream>
#include "BoxCollider.h"

class GameObject : public BoxCollider
{
public:

	GameObject();
	~GameObject();																								//all the different public functions

	void SetMesh(std::shared_ptr<Mesh> input) { _mesh = input; }
	void SetMaterial(std::shared_ptr<Material> input) { _material = input; }

	std::shared_ptr<Material> GetMaterial() { return _material; }
	
	void SetPosition( float posX, float posY, float posZ ) {_position.x = posX; _position.y = posY; _position.z = posZ;}
	void SetRotation( float rotX, float rotY, float rotZ ) {_rotation.x = rotX; _rotation.y = rotY; _rotation.z = rotZ;}

	void SetChangePos(glm::vec3 _value) { _changeInPostion = _value; }
	void SetChangeRot(glm::vec3 _value) { _changeInRotation = _value; }

	double GetDistanceFromPlayer() { return distanceFromPlayer;  }

	void CalculateDistanceFromPlayer(glm::mat4 _viewMatrix);
	
	void Update( float deltaTs, glm::mat4 _viewMatrix);

	void Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix, bool isSkyBox, int _width, int _height);

	std::shared_ptr<BoxCollider> GetBoxCollider() { return boxCollider; }

	void Delete() { deleteMe = true; }
	bool ShouldIBeDeleted() { return deleteMe; }

protected:
	bool deleteMe;

	std::shared_ptr<Mesh> _mesh;
	std::shared_ptr<Material> _material;

	// Matrix for the position and orientation of the game object
	glm::mat4 _modelMatrix;
	glm::mat4 _invModelMatrix;
	
	// Position of the model														//private variables
	glm::vec3 _position;
	
	// Orientation of the model
	glm::vec3 _rotation;

	double distanceFromPlayer;

	glm::vec3 _changeInRotation;
	glm::vec3 _changeInPostion;

	std::shared_ptr<BoxCollider> boxCollider;
	
};



#endif
