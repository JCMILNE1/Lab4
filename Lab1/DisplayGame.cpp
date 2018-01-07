#include "DisplayGame.h"


DisplayGame::DisplayGame()
{
	ThisSDLWindow = nullptr; //initialise to generate null access violation for debugging. 
	screenWidth = 1024.0f;
	screenHeight = 768.0f; 
}

DisplayGame::~DisplayGame()
{
	SDL_GL_DeleteContext(ThisGLContext); // delete context
	SDL_DestroyWindow(ThisSDLWindow); // detete window (make sure to delete the context and the window in the opposite order of creation in initDisplay())
	SDL_Quit();
}

float DisplayGame::getScreenWidth() { return screenWidth; } //getters
float DisplayGame::getScreenHeight() { return screenHeight; }

void DisplayGame::StartDisplay()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initalise everything

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// set CameraUp z-buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set CameraUp double buffer   

	ThisSDLWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)screenWidth, (int)screenHeight, SDL_WINDOW_OPENGL); // create window

	if (ThisSDLWindow == nullptr)
	{
		errorReport("window failed to create");
	}

	ThisGLContext = SDL_GL_CreateContext(ThisSDLWindow);

	if (ThisGLContext == nullptr)
	{
		errorReport("SDL_GL context failed to create");
	}

	GLenum errorOccured = glewInit();
	if (errorOccured != GLEW_OK)
	{
		errorReport("GLEW failed to initialise");
	}

	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE); //dont draw faces that are not pointing at the camera

	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);
}

void DisplayGame::errorReport(std::string errorMessage)   //prints an error to the screen if one ocured
{
	std::cout << errorMessage << std::endl;
	std::cout << "press any  key to quit...";
	int i;
	std::cin >> i;
	SDL_Quit();
}


void DisplayGame::ClearGameScreen(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}

void DisplayGame::ChangeBuffers()
{
	SDL_GL_SwapWindow(ThisSDLWindow); //swap buffers
}

