#pragma once
#include "SDL2/include/SDL.h"
#include "glm.hpp"
#include "SDL_ttf.h"

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

   glm::vec2 PlayerPos = {15,525};

   glm::vec2 BallPos = {955,550};

   glm::vec2 EnemyPos = {1890,525};

   float deltatime;

   Uint32 MILLI_SEC_PREVIOUS_FRAME;
   
private:

	void PlayerMovement();
	void BallMovement();
	void EnemyMovement();
	void BallHitsPaddle();
	
	void LoadFontOnScreen();

	int thickness = 15;

	//Window

	SDL_Window* myWind;
	SDL_Renderer* myRen;

	TTF_Font* myFont;

	SDL_Surface* myFontSurface;
	SDL_Texture* myFontTexture;

	SDL_Rect PlayerPaddleRect;

	SDL_Rect EnemyPaddleRect;
	
	SDL_Rect BallRect;

	SDL_Rect ScoreFont;

	//Walls

	SDL_Rect TopWallRect;
	SDL_Rect BottomWallRect;
	SDL_Rect LeftWallRect;
	SDL_Rect RightWallRect;
	
	

	bool bisMovingUp;
	bool bisMovingDown;
	

	bool PlayerHitBall;

	int speed = 800;
	int Score = 0;

	float BallVelX = 600.0f;
	float BallVelY = 600.0f;

	float EnemyVelY = 500.0f;
};

