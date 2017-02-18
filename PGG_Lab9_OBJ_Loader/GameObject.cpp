
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "GameObject.h"

GameObject::GameObject()
{
	// Initialise everything here
	_mesh = NULL;
	_material = NULL;
	_changeInPostion = glm::vec3(0, 0, 0);
	_changeInRotation = glm::vec3(0, 0.5, 0);
}

GameObject::~GameObject()
{
	// Do any clean up here
}

void GameObject::Update(float deltaTs, glm::mat4 _viewMatrix)
{
	// Put any update code here
	// Change the _position and _rotation to move the model


	_rotation += _changeInRotation * deltaTs;
	_position += _changeInPostion * deltaTs;

	//_rotation.y += deltaTs * 0.5f;
	while (_rotation.y > (3.14159265358979323846 * 2.0))
	{
		_rotation.y -= (float)(3.14159265358979323846 * 2.0);
	}


	CalculateDistanceFromPlayer(_viewMatrix);

}

void GameObject::Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix)
{

	if (_mesh != NULL)
	{
		if (_material != NULL)
		{

			// Make sure matrices are up to date (if you don't change them elsewhere, you can put this in the update function)
			_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0, 1, 0));
			_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.y, glm::vec3(0, 1, 0));

			// Give all the matrices to the material
			// This makes sure they are sent to the shader
			_material->SetMatrices(_modelMatrix, _invModelMatrix, viewMatrix, projMatrix);
			// This activates the shader
			_material->Apply();
		}

		// Sends the mesh data down the pipeline
		_mesh->Draw();

	}

}



void GameObject::CalculateDistanceFromPlayer(glm::mat4 _viewMatrix)
{
	glm::vec4 _cameraPosition = glm::inverse(_viewMatrix) * glm::vec4(0, 0, 0, 1);


	double x = pow(_cameraPosition.x - _position.x, 2);
	double y = pow(_cameraPosition.y - _position.y, 2);
	double z = pow(_cameraPosition.z - _position.z, 2);

	//std::cout << _cameraPosition.x << "," << _cameraPosition.y << "," << _cameraPosition.z << std::endl;

	distanceFromPlayer = pow(x + y + z, 0.5);
}
