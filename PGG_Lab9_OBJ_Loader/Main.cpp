
//just a note: I spent a good week trying to implement multiple frame buffers. However I just could not get the 2d texture which is created when you do the first pass through the shaders
//to be drawn on the quad that displays everything. Therefore it does not render using multiple frame buffers, however I have kept the code in place but commented it out

//   PASS THE VERTICES THROUGH THE SHADER (basically to get rid of unwanted fragments that wont be displayed ->>> PUT THE GENERATED 'IMAGE' ONTO A QUAD ->> PASS 'IMAGE' BACK THROUGH AND DO LIGHTING CALC
//																													^^ i think it was going wrong here

//								the benefit of multiple FBO's is to save computational time for multiple lights as you aren't processing lighting calculations for unnecerssary fragments


//             In order to play the tech demo thing use wasd to control and look around using the mouse (is currently affected by gimbal lock) or the arrow keys 

#include <SDL.h>
#include "glew.h"
#include "Scene.h"
#include <iostream>			//the include files we are using
#include <string>
#include "FrameBuffer.h"

bool InitGL()
{
	glewExperimental = GL_TRUE;

	GLenum err = glewInit();																										//initlaise glew
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cerr << "Error: GLEW failed to initialise with message: " << glewGetErrorString(err) << std::endl;
		return false;
	}
	std::cout << "INFO: Using GLEW " << glewGetString(GLEW_VERSION) << std::endl;									
																																		//output to the console some version information
	std::cout << "INFO: OpenGL Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "INFO: OpenGL Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "INFO: OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return true;
}


int main(int argc, char *argv[])
{
	// This is our initialisation phase

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Cannot initialise SDL." << std::endl;
		return -1;
	}

	//set context profile 4.3
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	// and use core profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//initalise SDL

	int winPosX = 100;
	int winPosY = 100;
	int winWidth = 640;
	int winHeight = 640;
	SDL_Window *window = SDL_CreateWindow("Space Teapot explorer Demo", winPosX, winPosY, winWidth, winHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);		//create a new renderer

	//create a context so opengl can draw
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);

	if (!InitGL())
	{
		return -1;
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);		//used so the mouse gets locked into the screen (esc to get it back out)

	unsigned int lastTime = SDL_GetTicks();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	Scene myScene(winWidth, winHeight);			//create a new scene


	//CONTROL VARIABLES
	bool cmdRotateLeft = false, cmdRotateRight = false, cmdRotateUp = false, cmdRotateDown = false;
	bool aKeyDown = false, sKeyDown = false, dKeyDown = false, wKeyDown = false;

	int oldXMousePos = 0, oldYMousePos = 0;			//used to track the mouse movement
	int newXMousePos = 0, newYMousePos = 0;
	float rotationAngle = 0.0f;


	bool go = true;
	while (go)
	{


		SDL_Event incomingEvent;		//create an event storer

		while (SDL_PollEvent(&incomingEvent))		//if there is an incoming event
		{
			switch (incomingEvent.type)				//get the incoming event type
			{
			case SDL_QUIT:
				go = false;								//close the screen
				break;
			case SDL_KEYDOWN:									//if a button has been pressed
				switch (incomingEvent.key.keysym.sym)
				{
				case SDLK_DOWN:
					cmdRotateDown = true;
					break;
				case SDLK_UP:
					cmdRotateUp = true;
					break;
				case SDLK_LEFT:
					cmdRotateLeft = true;
					break;
				case SDLK_RIGHT:							//set some bool variables so we know which key
					cmdRotateRight = true;
					break;
				case SDLK_a:
					aKeyDown = true;
					break;
				case SDLK_d:
					dKeyDown = true;
					break;
				case SDLK_w:
					wKeyDown = true;
					break;
				case SDLK_s:
					sKeyDown = true;
					break;
				case SDLK_ESCAPE:
					SDL_SetRelativeMouseMode(SDL_FALSE);		//this is so we can close the program or get our mouse back
					break;
				}
				break;

			case SDL_KEYUP:
				switch (incomingEvent.key.keysym.sym)
				{
				case SDLK_DOWN:
					cmdRotateDown = false;
					break;
				case SDLK_UP:
					cmdRotateUp = false;
					break;
				case SDLK_LEFT:
					cmdRotateLeft = false;
					break;
				case SDLK_RIGHT:
					cmdRotateRight = false;					//reset the variables so it doesnt stick
					break;
				case SDLK_a:
					aKeyDown = false;
					break;
				case SDLK_d:
					dKeyDown = false;
					break;
				case SDLK_w:
					wKeyDown = false;
					break;
				case SDLK_s:
					sKeyDown = false;
					break;
				}
				break;
			case SDL_MOUSEMOTION:

				SDL_GetMouseState(&newXMousePos, &newYMousePos);											//get the mouse state

				break;
			case SDL_MOUSEBUTTONDOWN:
				myScene.SpawnShooter();																		//if you click then spawn a teapot
				break;
			}
		}


		unsigned int current = SDL_GetTicks();
		float deltaTs = (float)(current - lastTime) / 1000.0f;															//work out the timing between each frame 
		lastTime = current;

		if (aKeyDown == true)
		{
			myScene.ChangeCameraPosition(glm::vec3(2, 0, 0) * deltaTs);
		}
		if (sKeyDown == true)
		{
			myScene.ChangeCameraPosition(glm::vec3(0, 0, -2) * deltaTs);
		}
		if (dKeyDown == true)
		{
			myScene.ChangeCameraPosition(glm::vec3(-2, 0, 0) * deltaTs);
		}																						//control the camera with the wasd and arrow keys
		if (wKeyDown == true)
		{
			myScene.ChangeCameraPosition(glm::vec3(0, 0, 2) * deltaTs);
		}
		if (cmdRotateLeft == true)
		{
			newXMousePos -= 2;
		}
		if (cmdRotateRight == true)
		{
			newXMousePos += 2;
		}
		if (cmdRotateDown == true)
		{
			newYMousePos += 2;
		}
		if (cmdRotateUp == true)
		{
			newYMousePos -= 2;
		}

		myScene.ChangeInMousePosition(glm::vec2((newXMousePos - oldXMousePos), (newYMousePos - oldYMousePos)) * deltaTs);		//i think this still included gimbal lock

		oldXMousePos = newXMousePos;
		oldYMousePos = newYMousePos;		//reset the mouse differntial

		myScene.Update(deltaTs);			//update the scene

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);							//clear
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//now draw
		myScene.Draw();

		SDL_GL_SwapWindow(window);						//swap between the back and front buffer


		// Limiter in case we're running really quick
		if (deltaTs < (1.0f / 50.0f))	// not sure how accurate the SDL_Delay function is..
		{
			SDL_Delay((unsigned int)(((1.0f / 50.0f) - deltaTs)*1000.0f));
		}
	}

	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
