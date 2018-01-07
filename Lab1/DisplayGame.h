#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class DisplayGame
{
public:
	DisplayGame();
	~DisplayGame();
	void StartDisplay();
	void ChangeBuffers();
	void ClearGameScreen(float r, float g, float b, float a);

	float getScreenWidth();
	float getScreenHeight();

private:

	void errorReport(std::string errorMessage);
	
	SDL_GLContext ThisGLContext; //global variable to hold the context
	SDL_Window* ThisSDLWindow; //holds pointer to out window
	float screenHeight;
	float screenWidth;
	
};

