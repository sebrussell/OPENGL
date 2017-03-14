
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include "GameObject.h"

GameObject::GameObject()
{
	_mesh = NULL;
	_material = NULL;
	_changeInPostion = glm::vec3(0, 0, 0);										//initalise the variables in the contructor
	_changeInRotation = glm::vec3(0, 0.5, 0);
	boxCollider = std::make_shared<BoxCollider>(BoxCollider());
	deleteMe = false;
}

GameObject::~GameObject()
{
}

void GameObject::Update(float deltaTs, glm::mat4 _viewMatrix)
{
	boxCollider->UpdateCollider(_position, boxCollider);				//call the box collider update functions

	_rotation += _changeInRotation * deltaTs;							//change the position and rotation by how much it should change by each frame
	_position += _changeInPostion * deltaTs;

	while (_rotation.y > (3.14159265358979323846 * 2.0))
	{
		_rotation.y -= (float)(3.14159265358979323846 * 2.0);	
	}


	CalculateDistanceFromPlayer(_viewMatrix);							//calculate the distance from the player

}

void GameObject::Draw(glm::mat4 viewMatrix, glm::mat4 projMatrix, bool isSkyBox, int _width, int _height)
{
	if (_mesh != NULL)
	{
		if (_material != NULL)
		{
			if (isSkyBox == false)		//if it is not a sky box
			{
				_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
				_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0, 1, 0));					//apply the defualt translations to put it in place
				_invModelMatrix = glm::rotate(glm::mat4(1.0f), -_rotation.y, glm::vec3(0, 1, 0));
			}
			else
			{
				projMatrix = glm::perspective(45.0f, (float)_width / (float)_height, 0.1f, 100.0f);
				viewMatrix = glm::mat4(glm::mat3(viewMatrix));												//this removes any movement translations so you can never reach the end of the skybox
				_modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(50, 50, 50));
			}		

			_modelMatrix = glm::translate(glm::mat4(1.0f), _position);
			_modelMatrix = glm::rotate(_modelMatrix, _rotation.y, glm::vec3(0, 1, 0));


			_material->SetMatrices(_modelMatrix, _invModelMatrix, viewMatrix, projMatrix);					//apply
			_material->Apply();
		}
		_mesh->Draw();																						//draw mesh

	}

}

void GameObject::CalculateDistanceFromPlayer(glm::mat4 _viewMatrix)
{
	glm::vec4 _cameraPosition = glm::inverse(_viewMatrix) * glm::vec4(0, 0, 0, 1);


	double x = pow(_cameraPosition.x - _position.x, 2);
	double y = pow(_cameraPosition.y - _position.y, 2);			
	double z = pow(_cameraPosition.z - _position.z, 2);

	//std::cout << _cameraPosition.x << "," << _cameraPosition.y << "," << _cameraPosition.z << std::endl;

	distanceFromPlayer = pow(x + y + z, 0.5);																		//uses pythtagoras to work out the distance from the player
}
