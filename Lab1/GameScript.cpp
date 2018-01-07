#include "GameScript.h"
#include "Camera.h"
#include <iostream>
#include <string>






GameScript::GameScript()
{
	GameState = GameState::PLAYING;
	DisplayGame* GameScreen = new DisplayGame(); //creates a new display
    MeshHandler* Player();
	MeshHandler* Bullet();
}

GameScript::~GameScript()
{
}

void GameScript::runGame()
{
	initaliseGameSystems(); 
	runGameLoop();
}

void GameScript::inputProcessing()
{
	left = false;
	right = false;
	space = false;

	//this loop gets and proceses player input, alowing controll of the player and the camera
	SDL_Event Keypressed;
	while (SDL_PollEvent(&Keypressed)) 
	{
		if (Keypressed.type == SDL_KEYDOWN)
		{
			if (Keypressed.key.keysym.sym == SDLK_LEFT)
			{
				cout << "working";
				left = true;
			}

			if (Keypressed.key.keysym.sym == SDLK_RIGHT)
			{
				cout << "working";
				right = true;
			}

			if (Keypressed.key.keysym.sym == SDLK_SPACE)
			{
				cout << "working";
				space = true;
			}

			if (Keypressed.key.keysym.sym == SDLK_q)
			{
				PlayerCamera.Turn(0.05);
			}

			if (Keypressed.key.keysym.sym == SDLK_e)
			{
				PlayerCamera.Turn(-0.05);
			}

			if (Keypressed.key.keysym.sym == SDLK_w)
			{
				PlayerCamera.ForwardBack(0.05);
			}

			if (Keypressed.key.keysym.sym == SDLK_s)
			{
				PlayerCamera.ForwardBack(-0.05);
			}

			if (Keypressed.key.keysym.sym == SDLK_a)
			{
				PlayerCamera.RightLeft(0.05);
			}

			if (Keypressed.key.keysym.sym == SDLK_d)
			{
				PlayerCamera.RightLeft(-0.05);
			}

			if (Keypressed.key.keysym.sym == SDLK_UP)
			{
				PlayerCamera.Pitch(0.05);
			}

			if (Keypressed.key.keysym.sym == SDLK_DOWN)
			{
				PlayerCamera.Pitch(-0.05);
			}
		}
	}
}


void GameScript::runGameLoop()  //runs the game loop
{
	while (GameState != GameState::EXITGAME)
	{
		inputProcessing();  //gets te players inputs
		drawScreen();   //draws the game to the screen
		model1prevpos = Player.getSpherePosition();  //gets the previous position for the models
		model2prevpos = Bullet.getSpherePosition();
		model3prevpos = ModelMesh3.getSpherePosition();
		model4prevpos = ModelMesh4.getSpherePosition();
		playSound(SoundMusic, glm::vec3(0.0f, 0.0f, 0.0f));  //plays the background music
	}
}

void GameScript::drawScreen()
{
	GameScreen.ClearGameScreen(0.0f, 0.0f, 0.0f, 1.0f);
	
	ShaderHandler currentShader("..\\res\\shader"); //new shader
	TextureHandler texture1("..\\res\\bricks.jpg"); //load texture
	TextureHandler texture2("..\\res\\water.jpg"); //load texture
	//==================================================================
	if(left == true)                                                    //if a is pressed moves the player left
	{
	objectTransform.SetPos(model1prevpos + glm::vec3(0.1,0,0));
	objectTransform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	objectTransform.SetScale(glm::vec3(0.5, 0.5, 0.5));
	}
	else if (right == true)                                            //if d is pressed moves the player right
	{
		objectTransform.SetPos(model1prevpos + glm::vec3(-0.1, 0, 0));
		objectTransform.SetRot(glm::vec3(0.0, 0.0, 0.0));
		objectTransform.SetScale(glm::vec3(0.5, 0.5, 0.5));
	}
	else                                                            //if neither are pressed keeps the player in place
	{
		objectTransform.SetPos(model1prevpos);
		objectTransform.SetRot(glm::vec3(0.0, 0.0, 0.0));
		objectTransform.SetScale(glm::vec3(0.5, 0.5, 0.5));
	}

	currentShader.ShaderBinding();                                 //aplies position ans texture
	currentShader.UpdateShaders(objectTransform, PlayerCamera);
	texture2.BindTexture(0);
	Player.drawMesh();
	Player.updateSphereData(*objectTransform.GetPosition(), 0.6f);

	//==================================================================
	if (Bullet.getSpherePosition().y > 4 && space == true)                   //if the bullet id off screen, allows the player to move it to their position
	{
		objectTransform.SetPos(Player.getSpherePosition());
	}
	else                                                                 //moves the bullet up
	{
		objectTransform.SetPos(model2prevpos + glm::vec3(0, 0.2, 0));
		objectTransform.SetRot(glm::vec3(0.0, 0.0, 0.0));
		objectTransform.SetScale(glm::vec3(0.5, 0.5, 0.5));
	}

	currentShader.ShaderBinding();
	currentShader.UpdateShaders(objectTransform, PlayerCamera);
	texture2.BindTexture(0);
	Bullet.drawMesh();
	Bullet.updateSphereData(*objectTransform.GetPosition(), 0.6f);
	
	//==================================================================
	if (model3prevpos.x < -2)    //makes the enemy switch direction when it reaches the edge of the screen
	{
		enemy1left = true;
	}
	else
	if (model3prevpos.x > 2)
	{
		enemy1left = false;
	}

	if(enemy1left == true)
	{
		objectTransform.SetPos(model3prevpos + glm::vec3(0.1,0,0));
		objectTransform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
		objectTransform.SetScale(glm::vec3(0.25, 0.25, 0.25));
	}
	else
	{
		objectTransform.SetPos(model3prevpos - glm::vec3(0.1, 0, 0));
		objectTransform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
		objectTransform.SetScale(glm::vec3(0.25, 0.25, 0.25));
		
	}
	if (collisionProcessing(ModelMesh3.getSpherePosition(), ModelMesh3.getSphereRadius(), Bullet.getSpherePosition(), Bullet.getSphereRadius()) == true)  //if the bullet hits the position is reset and a sound plays
	{
		
		playSound(SoundEffect, glm::vec3(0.0f, 0.0f, 0.0f));
		objectTransform.SetPos(glm::vec3(-2, 0, 0));
	}
	currentShader.ShaderBinding();
	currentShader.UpdateShaders(objectTransform, PlayerCamera);
	texture1.BindTexture(0);
	ModelMesh3.drawMesh();
	ModelMesh3.updateSphereData(*objectTransform.GetPosition(), 0.6f);
	counter = counter + 0.01f;

	//==================================================================

	if (model4prevpos.x < -2)
	{
		enemy2left = true;
	}
	else
		if (model4prevpos.x > 2)
		{
			enemy2left = false;
		}

	if (enemy2left == true)
	{
		objectTransform.SetPos(model4prevpos + glm::vec3(0.1, 0, 0));
		objectTransform.SetRot(glm::vec3(0.0, 0.0, counter * -5));
		objectTransform.SetScale(glm::vec3(0.25, 0.25, 0.25));
	}
	else
	{
		objectTransform.SetPos(model4prevpos - glm::vec3(0.1, 0, 0));
		objectTransform.SetRot(glm::vec3(0.0, 0.0, counter * -5));
		objectTransform.SetScale(glm::vec3(0.25, 0.25, 0.25));

	}

	if (collisionProcessing(ModelMesh4.getSpherePosition(), ModelMesh4.getSphereRadius(), Bullet.getSpherePosition(), Bullet.getSphereRadius()) == true)
	{
		playSound(SoundEffect, glm::vec3(0.0f, 0.0f, 0.0f));
		objectTransform.SetPos(glm::vec3(2, 2, 0));
		
	}
	currentShader.ShaderBinding();
	currentShader.UpdateShaders(objectTransform, PlayerCamera);
	texture1.BindTexture(0);
	ModelMesh4.drawMesh();
	ModelMesh4.updateSphereData(*objectTransform.GetPosition(), 0.6f);
	counter = counter + 0.01f;

	//==================================================================
				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	GameScreen.ChangeBuffers();
} 

void GameScript::initaliseGameSystems()
{//sets up the game systems and initial positions of the models
	GameScreen.StartDisplay(); 
	Player.LoadMeshFromFile("..\\res\\paddle4.obj");
	Bullet.LoadMeshFromFile("..\\res\\bullet2.obj");
	ModelMesh3.LoadMeshFromFile("..\\res\\monkey3.obj");
	ModelMesh4.LoadMeshFromFile("..\\res\\monkey3.obj");
	PlayerCamera.CreateGameCamera(glm::vec3(0, 0, -5), 70.0f, (float)GameScreen.getScreenWidth()/GameScreen.getScreenHeight(), 0.01f, 1000.0f);
	counter = 1.0f;
	SoundEffect = Soundmanager.ReadyClip("..\\res\\bang.wav");
	SoundMusic = Soundmanager.ReadyClip("..\\res\\background.wav");
	model1prevpos = glm::vec3(2, -2, 0);
	model2prevpos = glm::vec3(0, 100, 0);
	model3prevpos = glm::vec3(-2, 0, 0);
	model4prevpos = glm::vec3(2, 2, 0);
}

void GameScript::playSound(unsigned int Source, glm::vec3 pos)
{

	ALint AudioState;
	alGetSourcei(Source, AL_SOURCE_STATE, &AudioState);
	if (AL_PLAYING != AudioState)
	{
		Soundmanager.ActivateClip(Source, pos);
	}
}

bool GameScript::collisionProcessing(glm::vec3 Model1Position, float Model1Radius, glm::vec3 Model2Position, float Model2Radius) //checks for collisions
{
	float distanceBetweenModels = glm::sqrt((Model2Position.x - Model1Position.x)*(Model2Position.x - Model1Position.x) + (Model2Position.y - Model1Position.y)*(Model2Position.y - Model1Position.y) + (Model2Position.z - Model1Position.z)*(Model2Position.z - Model1Position.z));

	if (distanceBetweenModels < (Model1Radius + Model2Radius))
	{
		return true;
	}
	else
	{
		return false;
	}
}

