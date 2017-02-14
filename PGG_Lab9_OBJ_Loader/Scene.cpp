
#include "Scene.h"

#include <iostream>
#include <SDL.h>



Scene::Scene()
{
	_cameraAngleX = 0.0f, _cameraAngleY = 0.0f;

	// Set up the viewing matrix
	// This represents the camera's orientation and position
	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1.0f, -3.5f));

	_viewDistance = 50;

	// Set up a projection matrix
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	// Set up your scene here

	// Position of the light, in world-space



	// Create a game object
	// This needs a material and a mesh

	//Position of camera
	//_cameraPosition = glm::vec3(0, 0, -3.5f);

	for (size_t y = 0; y < 20; y++)
	{
		//std::vector<GameObject*> tempRow;
		std::vector<std::shared_ptr<GameObject>> tempRow;
		for (size_t x = 0; x < 20; x++)
		{
			tempRow.push_back(std::make_shared<GameObject>());
		}
		_models.push_back(tempRow);
	}

	// Create the material for the game object
	//Material *modelMaterial = new Material();
	std::shared_ptr<Material> modelMaterial(new Material);
	// Shaders are now in files
	modelMaterial->LoadShaders("VertShader.txt", "FragShader.txt");
	// You can set some simple material properties, these values are passed to the shader
	// This colour modulates the texture colour

	// The material currently supports one texture
	// This is multiplied by all the light components (ambient, diffuse, specular)
	// Note that the diffuse colour set with the line above will be multiplied by the texture colour
	// If you want just the texture colour, use modelMaterial->SetDiffuseColour( glm::vec3(1,1,1) );
	modelMaterial->SetTexture("Image1.bmp");
	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	//modelMaterial->SetLightPosition(_lightPosition);

	amountOfLights = 2;

	for (size_t i = 0; i < amountOfLights; i++)
	{
		_lights.push_back(std::make_shared<Light>());
	}

	_lights[1]->m_position.x = 10;
	_lights[1]->m_lightColour.y = 1;
	_lights[0]->m_lightCutoff = 100;



	modelMaterial->SetLightPosition(_lights);

	// Tell the game object to use this material

	// The mesh is the geometry for the object
	//Mesh *modelMesh = new Mesh();
	std::shared_ptr<Mesh> modelMesh(new Mesh);
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("teapot3.obj");
	// Tell the game object to use this mesh

	//modelMaterial->SetDiffuseColour(glm::vec3(1, 1, 1));

	for (size_t y = 0; y < _models.size(); y++)
	{
		for (size_t x = 0; x < _models[y].size(); x++)
		{
			_models[y][x]->SetMaterial(modelMaterial);
			_models[y][x]->SetMesh(modelMesh);
			_models[y][x]->SetRotation(1 * (x + 1), 0, 0.01);
			_models[y][x]->SetPosition(x, -5, y);
		}
	}

}

Scene::~Scene()
{
	// You should neatly clean everything up here
}

void Scene::Update(float deltaTs)
{
	// Update the game object (this is currently hard-coded to rotate)

	int randNumber = rand() % 3;

	if (randNumber == 0)
	{
		_lights[1]->m_lightColour = glm::vec3(1, 0, 0);
	}
	else if (randNumber == 1)
	{
		_lights[1]->m_lightColour = glm::vec3(0, 1, 0);
	}
	else
	{
		_lights[1]->m_lightColour = glm::vec3(0, 0, 1);
	}


	for (size_t y = 0; y < _models.size(); y++)
	{
		for (size_t x = 0; x < _models[y].size(); x++)
		{			
			_models[y][x]->Update(deltaTs * (y + 1), _viewMatrix);
			_models[y][x]->GetMaterial()->SetLightPosition(_lights);
		}
	}

	

	// This updates the camera's position and orientation

	_viewMatrix = glm::rotate(glm::mat4(1), _cameraPosition.x, glm::vec3(0, 1, 0)) * _viewMatrix;
	_viewMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, _cameraPosition.z)) * _viewMatrix;

	//modelMaterial->SetLightPosition(_lights);

	_cameraPosition = glm::vec3();  //reset the current change in camera position
}

void Scene::Draw()
{
	// Draw that model, giving it the camera's position and projection
	for (size_t y = 0; y < _models.size(); y++)
	{
		for (size_t x = 0; x < _models[y].size(); x++)
		{
			if (_models[y][x]->GetDistanceFromPlayer() < _viewDistance)
			{
				_models[y][x]->Draw(_viewMatrix, _projMatrix);
			}
		}
	}

}



