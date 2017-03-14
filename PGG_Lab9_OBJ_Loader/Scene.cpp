
#include "Scene.h"
#include <iostream>					//the include files
#include <SDL.h>



Scene::Scene(unsigned int _width, unsigned int _height)
{
	windowHeight = _height;
	windowWidth = _width;																				//gets the window with

	_deleteDistance = 200;																				//sets a delete object distance so it doesnt go too far away

	CreateScreenVAO();																					//create a screen VAO

	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));									//set the default position

	_viewDistance = 150;																				//dont draw objects this far away

	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);											//a project matrix

	frameBuffer.GenertateFBO(windowHeight, windowHeight);												//generate the frame buffer object

	amountOfLights = 2;																					//set the amount of lights

	for (size_t i = 0; i < amountOfLights; i++)
	{
		_lights.push_back(std::make_shared<Light>());
	}

	_lights[0]->m_position.x = 10;
	_lights[0]->m_position.y = 10;
	_lights[0]->m_lightType = Light::spotlight;															
	_lights[0]->m_lightCutoff = 100;

	_lights[1]->m_lightType = Light::global;
	_lights[1]->m_ambientColour = glm::vec3(0.3, 0.1, 0.1);


	CreateSkyBox();																						//create a skybox

	modelMaterial = std::make_shared<Material>();
	modelMesh = std::make_shared<Mesh>();
	modelMaterial->LoadShaders("VertShader.txt", "FragShader.txt", 0);										//set the default model mesh for most objects
	modelMaterial->LoadTexture("Image1.bmp");
	modelMaterial->SetLightPosition(_lights);
	modelMesh->LoadOBJ("teapot3.obj");																	//i wanted to use asteroids but i couldnt find any good free ones which are triangulated 
																										//and are free with a good texture
	
	srand(time(NULL));
	for (size_t i = 0; i < 100; i++)
	{
		int x = rand() % 100 - 50;																		//spawn the default teapot asteroid things
		int y = rand() % 100 - 50;	
		int z = rand() % 100 - 50;
		
		SpawnObject(glm::vec3(0, 0, 0), glm::vec3(x / 10, y / 10, z / 10), x, y, z);
	}

	//textureColourBuffer = generateAttachmentTexture(false, false);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourBuffer, 0);

	//std::shared_ptr<Mesh> quadMesh(new Mesh);
	//quadMesh->LoadOBJ("quad.obj");																				//this is used for the multiple frame buffer objects
	//std::shared_ptr<Material> quadMaterial(new Material);
	////quadMaterial->LoadShaders("VertShader.txt", "FragShader.txt", 0);
	//quadMaterial->LoadShaders("GeometryPassVertexShader.txt", "GeometryPassFragmentShader.txt", 1);
	////quadMaterial->SetTexture("Image1.bmp");
	//quadMaterial->SetLightPosition(_lights);
	//drawQuad.SetMesh(quadMesh);
	//drawQuad.SetPosition(10, 1.0f, 5);
	//drawQuad.SetMaterial(quadMaterial);
}

Scene::~Scene()
{
	// You should neatly clean everything up here
}

void Scene::Update(float deltaTs)
{
	//int randNumber = rand() % 3;
	//if (randNumber == 0)
	//{
	//	_lights[1]->m_lightColour = glm::vec3(1, 0, 0);
	//}
	//else if (randNumber == 1)													//if you want the lights to change colour like a disco
	//{
	//	_lights[1]->m_lightColour = glm::vec3(0, 1, 0);
	//}
	//else
	//{
	//	_lights[1]->m_lightColour = glm::vec3(0, 0, 1);
	//}


	for (size_t y = 0; y < _models.size(); y++)																							//for every model
	{
		_models[y]->Update(deltaTs, _viewMatrix);																						//update the model
		if (_models[y]->GetDistanceFromPlayer() > _deleteDistance)																	//if it goes out of range
		{
			_models[y]->Delete();																									//delete it
		}


		for (size_t i = 0; i < _models.size(); i++)																					//for every other model in the scene
		{
			if (y != i && _models[y]->ShouldIBeDeleted() == false && _models[i]->ShouldIBeDeleted())								//if it hasnt already been collided with this frame
			{
				if (_models[y]->CheckForCollision(_models[i]->GetBoxCollider(), _models[y]->GetBoxCollider()) == true)				//and it is colldiing with an object
				{
					_models[y]->Delete();																							//delete the object
				}
			}			
		}
	}

	for (size_t i = 0; i < _models.size(); i++)																						//go through each object
	{
		if (_models[i]->ShouldIBeDeleted() == true)																					//if it has been collided with
		{
			_models.erase(_models.begin() + i);																						//erase it
		}
	}
	


	// This updates the camera's position and orientation

	_viewMatrix = glm::rotate(glm::mat4(1), _changeInCameraRotation.x, glm::vec3(0, 1, 0)) * _viewMatrix;
	_viewMatrix = glm::rotate(glm::mat4(1), _changeInCameraRotation.y, glm::vec3(1, 0, 0)) * _viewMatrix;				//rotate

	_viewMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, _changeInCameraPosition.z)) * _viewMatrix;				//then translate
	_viewMatrix = glm::translate(glm::mat4(1), glm::vec3(_changeInCameraPosition.x, 0, 0)) * _viewMatrix;

	//modelMaterial->SetLightPosition(_lights);																			//if the lights were to change during the program call this

	_changeInCameraPosition = glm::vec3();  //reset the current change in camera position
	_changeInCameraRotation = glm::vec2();
}

void Scene::Draw()
{
	
	glDepthMask(GL_FALSE);
	skyBox.GetMaterial()->ChangeShader(3);
	skyBox.Draw(_viewMatrix, _projMatrix, true, windowWidth, windowHeight);						//draws the skybox
	glDepthMask(GL_TRUE);


	//FirstPass();																				//set it up to use multiple frame buffers

	for (size_t y = 0; y < _models.size(); y++)
	{
		if (_models[y]->GetDistanceFromPlayer() < _viewDistance)								//if they arent too far away
		{
			_models[y]->GetMaterial()->ChangeShader(0);
			_models[y]->Draw(_viewMatrix, _projMatrix, false, windowWidth, windowHeight);			//draw all the other objects
		}
	}

	


	//drawQuad.Draw(_viewMatrix, _projMatrix);														//used when using multiple frame buffer objects

	//SecondPass();																					//^^^^^
}	

void Scene::FirstPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.GetFBO());
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
	glEnable(GL_DEPTH_TEST);
}

void Scene::SecondPass()
{

	//drawQuad.GetMaterial()->ChangeShader(1);
	////drawQuad.GetMaterial()->ChangeShader(0);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	//glClearColor(1.0f, 2.0f, 1.0f, 1.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//drawQuad.GetMaterial()->SetTexture(textureColourBuffer);									//this is used for the mutliple frame buffer objects
	//drawQuad.Draw(_viewMatrix, _projMatrix);
	//glGetError();


	/*glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, textureColourBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);*/
}

void Scene::CreateScreenVAO()
{
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);																			//makes a quad
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);							//adds the quad to a buffer data		
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);					
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

GLuint Scene::generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

															//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Using both a stencil and depth test, needs special format arguments
	if (!depth && !stencil)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, windowWidth, windowHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	}		
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	}		
		
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);			//make some opengl parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void Scene::SpawnObject(glm::vec3 changeInPos, glm::vec3 changeInRot, int xPos, int yPos, int zPos)
{
	_models.push_back(std::make_shared<GameObject>());
	_models[_models.size() - 1]->SetMaterial(modelMaterial);
	_models[_models.size() - 1]->SetMesh(modelMesh);
	_models[_models.size() - 1]->SetUpCollider(_models[_models.size() - 1]->GetBoxCollider(), modelMesh->GetModelWidth());		//this gets all the values and sets up an object
	_models[_models.size() - 1]->SetPosition(xPos, yPos, zPos);
	_models[_models.size() - 1]->SetChangePos(changeInPos);
	_models[_models.size() - 1]->SetChangeRot(changeInRot);
}

void Scene::SpawnShooter()
{
	glm::mat4 viewModel = glm::inverse(_viewMatrix);
	glm::vec3 cameraPos(viewModel[3]);
	glm::vec3 cameraFace(viewModel[2]);						//this gets the direction that the camera is facing
	cameraFace *= glm::vec3(10);
	//glm::vec3 cameraRot()
	SpawnObject(-cameraFace, glm::vec3(0, 0, 0), cameraPos.x, cameraPos.y - 1, cameraPos.z);		//and spawns an object
}

void Scene::CreateSkyBox()
{
	skyboxMaterial = std::make_shared<Material>();
	skyboxMesh = std::make_shared<Mesh>();
	skyboxMaterial->LoadShaders("CubeBoxVertShader.txt", "CubeBoxFragShader.txt", 3);
	skyboxMaterial->SetAsCubeMap(windowWidth, windowHeight);
	skyboxMaterial->SetLightPosition(_lights);														//this sets us the mesh and material for the skybox gameobject
	skyboxMesh->SetAsCube();
	skyBox.SetMaterial(skyboxMaterial);
	skyBox.SetMesh(skyboxMesh);
	skyBox.SetPosition(0, 0, 0);
}