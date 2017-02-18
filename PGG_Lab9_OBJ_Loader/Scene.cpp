
#include "Scene.h"

#include <iostream>
#include <SDL.h>



Scene::Scene(unsigned int _width, unsigned int _height)
{
	windowHeight = _height;
	windowWidth = _width;

	_deleteDistance = 200;

	CreateScreenVAO();
	// Set up the viewing matrix
	// This represents the camera's orientation and position
	_viewMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-10, 1.0f, -13.5f));

	_viewDistance = 50;

	// Set up a projection matrix
	_projMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 100.0f);

	// Set up your scene here

	// Position of the light, in world-space

	frameBuffer.GenertateFBO(windowHeight, windowHeight);

	// Create a game object
	// This needs a material and a mesh

	//Position of camera
	//_cameraPosition = glm::vec3(0, 0, -3.5f);


	// Create the material for the game object
	//Material *modelMaterial = new Material();

	// Need to tell the material the light's position
	// If you change the light's position you need to call this again
	//modelMaterial->SetLightPosition(_lightPosition);

	amountOfLights = 1;

	for (size_t i = 0; i < amountOfLights; i++)
	{
		_lights.push_back(std::make_shared<Light>());
	}

	_lights[0]->m_position.x = 10;
	_lights[0]->m_position.y = 10;
	_lights[0]->m_lightColour.y = 1;
	_lights[0]->m_lightType = Light::spotlight;
	_lights[0]->m_lightCutoff = 100;

	//_lights[0]->m_lightType = Light::global;
	//_lights[0]->m_ambientColour = glm::vec3(1, 1, 1);


	modelMaterial = std::make_shared<Material>();
	modelMesh = std::make_shared<Mesh>();
	modelMaterial->LoadShaders("VertShader.txt", "FragShader.txt", 0);
	modelMaterial->SetTexture("Image1.bmp");
	modelMaterial->SetLightPosition(_lights);
	modelMesh->LoadOBJ("teapot3.obj");

	// Tell the game object to use this mesh

	//modelMaterial->SetDiffuseColour(glm::vec3(1, 1, 1));

	for (size_t y = 0; y < 20; y++)
	{
		for (size_t x = 0; x < 20; x++)
		{
			SpawnObject(glm::vec3(0, 0, 0), glm::vec3(0.5, 0.1, 0), x, -5, y);
		}
	}

	//textureColourBuffer = generateAttachmentTexture(false, false);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColourBuffer, 0);

	//std::shared_ptr<Mesh> quadMesh(new Mesh);
	//quadMesh->LoadOBJ("quad.obj");
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
	// Update the game object (this is currently hard-coded to rotate)

	//int randNumber = rand() % 3;

	//if (randNumber == 0)
	//{
	//	_lights[1]->m_lightColour = glm::vec3(1, 0, 0);
	//}
	//else if (randNumber == 1)
	//{
	//	_lights[1]->m_lightColour = glm::vec3(0, 1, 0);
	//}
	//else
	//{
	//	_lights[1]->m_lightColour = glm::vec3(0, 0, 1);
	//}


	for (size_t y = 0; y < _models.size(); y++)
	{
		_models[y]->Update(deltaTs, _viewMatrix);
		if (_models[y]->GetDistanceFromPlayer() > _deleteDistance)
		{
			_models.erase(y + _models.begin());
		}
	}

	

	// This updates the camera's position and orientation

	_viewMatrix = glm::rotate(glm::mat4(1), _changeInCameraRotation.x, glm::vec3(0, 1, 0)) * _viewMatrix;
	_viewMatrix = glm::rotate(glm::mat4(1), _changeInCameraRotation.y, glm::vec3(1, 0, 0)) * _viewMatrix;

	//_viewMatrix = glm::rotate(glm::mat4(1), _cameraPosition.y, glm::vec3(1, 0, 0)) * _viewMatrix;
	_viewMatrix = glm::translate(glm::mat4(1), glm::vec3(0, 0, _changeInCameraPosition.z)) * _viewMatrix;
	_viewMatrix = glm::translate(glm::mat4(1), glm::vec3(_changeInCameraPosition.x, 0, 0)) * _viewMatrix;

	//modelMaterial->SetLightPosition(_lights);

	_changeInCameraPosition = glm::vec3();  //reset the current change in camera position
	_changeInCameraRotation = glm::vec2();
}

void Scene::Draw()
{
	FirstPass();

	for (size_t y = 0; y < _models.size(); y++)
	{
		if (_models[y]->GetDistanceFromPlayer() < _viewDistance)
		{
			//_models[y]->GetMaterial()->ChangeShader(0);
			_models[y]->Draw(_viewMatrix, _projMatrix);
		}
	}

	
	//drawQuad.Draw(_viewMatrix, _projMatrix);

	//SecondPass();
}

void Scene::FirstPass()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.GetFBO());
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer now
	glEnable(GL_DEPTH_TEST);
}

void Scene::SecondPass()
{
	
	drawQuad.GetMaterial()->ChangeShader(1);
	//drawQuad.GetMaterial()->ChangeShader(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // back to default
	glClearColor(1.0f, 2.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
	drawQuad.GetMaterial()->SetTexture(textureColourBuffer);
	drawQuad.Draw(_viewMatrix, _projMatrix);
	glGetError();


	/*glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, textureColourBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);*/
}

//void Scene::SecondPass()
//{
//
//	static float a = 0, b = 0, c = 0;
//
//	const float aspect = (float)windowWidth / (float)windowHeight;
//
//	glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//	glViewport(0, 0, windowWidth, windowHeight);
//
//	glClearColor(1., 1., 1., 0.);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//glMatrixMode(GL_PROJECTION);
//	//glLoadIdentity();
//	//gluPerspective(45, aspect, 1, 10);
//
//	//glMatrixMode(GL_MODELVIEW);
//	//glLoadIdentity();
//	//glTranslatef(0, 0, -5);
//
//	//glRotatef(b, 0, 1, 0);
//
//	b = fmod(b + 0.5, 360.);
//
//	glEnable(GL_TEXTURE_2D);
//	glBindTexture(GL_TEXTURE_2D, textureColourBuffer);
//
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//
//	glEnable(GL_BLEND);
//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//
//	glDisable(GL_LIGHTING);
//
//	float cube[][5] =
//	{
//		{ -1, -1, -1,  0,  0 },
//		{ 1, -1, -1,  1,  0 },
//		{ 1,  1, -1,  1,  1 },
//		{ -1,  1, -1,  0,  1 },
//
//		{ -1, -1,  1, -1,  0 },
//		{ 1, -1,  1,  0,  0 },
//		{ 1,  1,  1,  0,  1 },
//		{ -1,  1,  1, -1,  1 },
//	};
//	unsigned int faces[] =
//	{
//		0, 1, 2, 3,
//		1, 5, 6, 2,
//		5, 4, 7, 6,
//		4, 0, 3, 7,
//		3, 2, 6, 7,
//		4, 5, 1, 0
//	};
//
//	glEnableClientState(GL_VERTEX_ARRAY);
//	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
//
//	glVertexPointer(3, GL_FLOAT, 5 * sizeof(float), &cube[0][0]);
//	glTexCoordPointer(2, GL_FLOAT, 5 * sizeof(float), &cube[0][3]);
//
//	glCullFace(GL_BACK);
//	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, faces);
//
//	glCullFace(GL_FRONT);
//	glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, faces);
//
//	glDisableClientState(GL_VERTEX_ARRAY);
//	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
//
//
//
//}

void Scene::CreateScreenVAO()
{
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

GLuint Scene::generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	// What enum to use?
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
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, windowWidth, windowHeight, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, windowWidth, windowHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void Scene::SpawnObject(glm::vec3 changeInPos, glm::vec3 changeInRot, int xPos, int yPos, int zPos)
{
	_models.push_back(std::make_shared<GameObject>());	
	_models.push_back(std::make_shared<GameObject>());
	_models[_models.size() - 1]->SetMaterial(modelMaterial);
	_models[_models.size() - 1]->SetMesh(modelMesh);
	_models[_models.size() - 1]->SetPosition(xPos, yPos, zPos);
	_models[_models.size() - 1]->SetChangePos(changeInPos);
	_models[_models.size() - 1]->SetChangeRot(changeInRot);
}

void Scene::SpawnShooter()
{
	glm::mat4 viewModel = glm::inverse(_viewMatrix);
	glm::vec3 cameraPos(viewModel[3]);
	glm::vec3 cameraFace(viewModel[2]);
	//glm::vec3 cameraRot()
	SpawnObject(-cameraFace, glm::vec3(0, 0, 0), cameraPos.x, cameraPos.y - 1, cameraPos.z);


}