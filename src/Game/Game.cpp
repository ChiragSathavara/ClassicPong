#include "Game.h"
#include <iostream>

Game::Game() : myWind(nullptr), myRen(nullptr)
{
	std::cout << "GAME CONSTRUCTOR CALLED" << std::endl;

	bisRunning = false;

	PlayerHitBall = false;
}

Game::~Game()
{
	std::cout << "GAME DESTRUCTOR CALLED" << std::endl;
	Destroy();
}

void Game::InitGame()
{
	std::cout << "Init Game CALLED" << std::endl;

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		return;
	}

	SDL_DisplayMode displaymode;
	SDL_GetCurrentDisplayMode(0, &displaymode);
	WindHeight = displaymode.h;
	WindWidth = displaymode.w;

	myWind = SDL_CreateWindow("G1_GAME_ENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindWidth, WindHeight,SDL_RENDERER_ACCELERATED);
	if (myWind == nullptr)
	{
		return;
	}
	myRen = SDL_CreateRenderer(myWind, 0, 0);

	if (myRen == nullptr)
	{
		return;
	}

	TopWallRect = { 0,0,WindWidth,thickness };

	BottomWallRect = { 0,WindHeight - thickness,WindWidth,thickness };

	LeftWallRect = { 0,0,thickness,WindHeight};

	RightWallRect = { WindWidth - thickness,0,thickness,WindHeight };

	PlayerPaddleRect = { static_cast<int>(PlayerPos.x),static_cast<int>(PlayerPos.y),thickness,80};
	
	BallRect = { static_cast<int>(BallPos.x - thickness/2),static_cast<int>(BallPos.y - thickness/2),thickness,thickness };

	bisRunning = true;

}

void Game::ProcessInput()
{
	SDL_Event myEvnt;
	while (SDL_PollEvent(&myEvnt))
	{
		switch (myEvnt.type)
		{
		case SDL_QUIT:
			bisRunning = false;
		break;
		}
	}

	const Uint8* myKeyboardState = SDL_GetKeyboardState(NULL);

	if (myKeyboardState[SDL_SCANCODE_ESCAPE])
	{
		bisRunning = false;
	}

	if (myKeyboardState[SDL_SCANCODE_W])
	{
		bisMovingUp = true;
	}
	else
	{
		bisMovingUp = false;
	}
	if (myKeyboardState[SDL_SCANCODE_S])
	{
		bisMovingDown = true;
	}
	else
	{
		bisMovingDown = false;
	}
}

void Game::Update()
{
	auto TimeToDelay = MILLI_SEC_FRAME - (SDL_GetTicks() - MILLI_SEC_PREVIOUS_FRAME);

	if (TimeToDelay > 0 && TimeToDelay <= MILLI_SEC_FRAME)
	{
		SDL_Delay(TimeToDelay);
	}

	auto Ticks = SDL_GetTicks();

	deltatime = (Ticks - MILLI_SEC_PREVIOUS_FRAME) / 1000.f;
	
	MILLI_SEC_PREVIOUS_FRAME = Ticks;

	PlayerMovement();
	BallMovement();
	BallHitsPaddle();
}

void Game::RunGameLoop()
{
	while (bisRunning == true)
	{
		ProcessInput();
		Update();
		Render();
	}
}

void Game::Render()
{
	SDL_SetRenderDrawColor(myRen, 21, 21, 21, 255);
	SDL_RenderClear(myRen);
	SDL_SetRenderDrawColor(myRen, 255, 255, 255, 255);
	
	//PlayerPaddleRect
	SDL_RenderFillRect(myRen, &PlayerPaddleRect);
	
	//BallRect
	SDL_RenderFillRect(myRen, &BallRect);

	//Changing Wall Color
	SDL_SetRenderDrawColor(myRen, 25, 50, 89, 255);
	
	//TopWallRect
	SDL_RenderFillRect(myRen, &TopWallRect);

	//BottomRect
	SDL_RenderFillRect(myRen, &BottomWallRect);

	//LeftWallRect
	SDL_RenderFillRect(myRen, &LeftWallRect);

	//RightWallRect
	SDL_RenderFillRect(myRen, &RightWallRect);

	SDL_RenderPresent(myRen);
}

void Game::Destroy()
{
	SDL_DestroyWindow(myWind);
	SDL_DestroyRenderer(myRen);
	SDL_Quit();
}

void Game::PlayerMovement()
{
	if (bisMovingUp == true)
	{
		if (PlayerPos.y < 0)
		{
			PlayerPos.y = 0;
		}
		PlayerPos.y -= speed * deltatime;
		std::cout << "Moving Up :- PlayerPos Y = " << PlayerPos.y << std::endl;
	}

	if (bisMovingDown == true)
	{
		if (PlayerPos.y + 80 > WindHeight)
		{
			PlayerPos.y = WindHeight - 80;
		}
		PlayerPos.y += speed * deltatime;
		std::cout << "Moving Down :- PlayerPos Y = " << PlayerPos.y << std::endl;
	}
	PlayerPaddleRect.y = PlayerPos.y;
	//std::cout << "PlayerPaddleRect Y :- " << PlayerPaddleRect.y << std::endl;

}
void Game::BallMovement()
{
	BallPos.x += BallVelX * deltatime;
	BallPos.y += BallVelY * deltatime;

	if (BallPos.x < 0)
	{
		BallPos.x = 0;
		BallVelX = -BallVelX;
	}

	if (BallPos.x + thickness > WindWidth)
	{
		BallPos.x = WindWidth - thickness;
		BallVelX = -BallVelX;
	}

	if (BallPos.y < 0)
	{
		BallPos.y = 0;
		BallVelY = -BallVelY;
	}

	if (BallPos.y + thickness > WindHeight)
	{
		BallPos.y = WindHeight - thickness;
		BallVelY = -BallVelY;
	}
	BallRect.x = BallPos.x;
	BallRect.y = BallPos.y;
}

void Game::BallHitsPaddle()
{
	if (SDL_HasIntersection(&PlayerPaddleRect, &BallRect) == true)
	{
		BallVelX = -BallVelX;
	}
}
