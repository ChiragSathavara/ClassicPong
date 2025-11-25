#pragma once
#include "SDL2/include/SDL.h"
#include "glm.hpp"

const int FPS = 60;
const int MILLI_SEC_FRAME = FPS / 1000;

class Game
{
public:
	Game();
   ~Game();

   void InitGame();
   void ProcessInput();
   void Update();
   void RunGameLoop();
   void Render();
   void Destroy();

   int WindHeight;
   int WindWidth;

   bool bisRunning;

   glm::vec2 PlayerPos = { 50,525 };

   glm::vec2 BallPos = { 800,550 };

   glm::vec2 EnemyPos = {1857,525};

   float deltatime;

   Uint32 MILLI_SEC_PREVIOUS_FRAME;
   
private:

	void PlayerMovement();
	void BallMovement();
	void EnemyMovement();
	void BallHitsPaddle();

	int thickness = 15;

	//Window

	SDL_Window* myWind;
	SDL_Renderer* myRen;
	
	SDL_Rect PlayerPaddleRect;

	SDL_Rect EnemyPaddleRect;
	
	SDL_Rect BallRect;

	//Walls

	SDL_Rect TopWallRect;
	SDL_Rect BottomWallRect;
	SDL_Rect LeftWallRect;
	SDL_Rect RightWallRect;
	
	

	bool bisMovingUp;
	bool bisMovingDown;
	

	bool PlayerHitBall;

	int speed = 800;
	

	float BallVelX = 600.0f;
	float BallVelY = 600.0f;

	float EnemyVelY = 500.0f;
};

