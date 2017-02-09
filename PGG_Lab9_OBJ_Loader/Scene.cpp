
#include "Scene.h"

#include <iostream>
#include <SDL.h>



Scene::Scene()
{
	_cameraAngleX = 0.0f, _cameraAngleY = 0.0f;

	// Set up the viewing matrix
	// This represents the camera's orientation and position
	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,-3.5f) );
	

	// Set up a projection matrix
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);
	
	// Set up your scene here

	// Position of the light, in world-space
	
	

	// Create a game object
	// This needs a material and a mesh

	//Position of camera
	_cameraPosition = glm::vec3(0, 0, -3.5f);

	for (size_t y = 0; y < 40; y++)
	{
		std::vector<GameObject*> tempRow;
		for (size_t x = 0; x < 40; x++)
		{		
			tempRow.push_back(nullptr);			
		}
		_models.push_back(tempRow);
	}

	for (size_t y = 0; y < _models.size(); y++)
	{
		for (size_t x = 0; x < _models[y].size(); x++)
		{
			_models[y][x] = new GameObject;
		}
	}
	

	// Create the material for the game object
	Material *modelMaterial = new Material();
	// Shaders are now in files
	modelMaterial->LoadShaders("VertShader.txt","FragShader.txt");
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

	for (size_t i = 0; i < modelMaterial->GetLightAmount(); i++)
	{
		_lights.push_back(new Light(10 * i, 50, 0));
	}

	modelMaterial->SetLightPosition(_lights);

	// Tell the game object to use this material

	// The mesh is the geometry for the object
	Mesh *modelMesh = new Mesh();
	// Load from OBJ file. This must have triangulated geometry
	modelMesh->LoadOBJ("teapot3.obj");
	// Tell the game object to use this mesh

	modelMaterial->SetDiffuseColour(glm::vec3(1, 1, 1));

	for (size_t y = 0; y < _models.size(); y++)
	{
		for (size_t x = 0; x < _models[y].size(); x++)
		{
			//modelMaterial->SetDiffuseColour(glm::vec3(0.8 / x, 0.1 * y, 0.1));
			
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

void Scene::Update( float deltaTs )
{
	// Update the game object (this is currently hard-coded to rotate)

	for (size_t y = 0; y < _models.size(); y++)
	{
		for (size_t x = 0; x < _models[y].size(); x++)
		{
			_models[y][x]->Update(deltaTs * (y + 1));
		}
	}
	

	// This updates the camera's position and orientation
	_viewMatrix = glm::rotate( glm::rotate( glm::translate( glm::mat4(1.0f), _cameraPosition ), _cameraAngleX, glm::vec3(1,0,0) ), _cameraAngleY, glm::vec3(0,1,0) );

	//_viewMatrix = glm::rotate(glm::translate(glm::rotate(glm::mat4(1.0f), _cameraAngleX, glm::vec3(1, 0, 0)), _cameraPosition), _cameraAngleY, glm::vec3(0, 1, 0));
}

void Scene::Draw()
{
	// Draw that model, giving it the camera's position and projection
	for (size_t y = 0; y < _models.size(); y++)
	{
		for (size_t x = 0; x < _models[y].size(); x++)
		{
			_models[y][x]->Draw(_viewMatrix, _projMatrix);
		}
	}

}



