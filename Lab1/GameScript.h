#pragma once
#include <SDL\SDL.h>
#include <GL/glew.h>
#include "DisplayGame.h" 
#include "ShaderHandler.h"
#include "MeshHandler.h"
#include "TextureHandler.h"
#include "ModelTransform.h"
#include "SoundManager.h"


enum class GameState{PLAYING, EXITGAME};

class GameScript
{
public:
	GameScript();
	~GameScript();

	void runGame();

private:

	void initaliseGameSystems();
	void inputProcessing();
	void runGameLoop();
	void drawScreen();
	bool collisionProcessing(glm::vec3 Model1Position, float Model1Radius, glm::vec3 Model2Position, float Model2Radius);
	void playSound(unsigned int Source, glm::vec3 pos);

	DisplayGame GameScreen;
	GameState GameState;
	MeshHandler Player;
	MeshHandler Bullet;
	MeshHandler ModelMesh3;
	MeshHandler ModelMesh4;
	Camera PlayerCamera;
	SoundManager Soundmanager;
	ModelTransform objectTransform;
	bool left;
	bool right;
	bool space;
	bool enemy1left;
	bool enemy2left;
	float counter;

	unsigned int SoundEffect;
	unsigned int SoundMusic;

	glm::vec3 model1prevpos;
	glm::vec3 model2prevpos;
	glm::vec3 model3prevpos;
	glm::vec3 model4prevpos;


};

