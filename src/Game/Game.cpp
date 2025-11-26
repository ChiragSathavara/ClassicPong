#include "Game.h"
#include <iostream>
#include <string>

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
	if (TTF_Init() == -1)
	{
		printf("TTF isnt Initialize!");
	}
	else
	{
		printf("TTF is Initialize!");
	}
	SDL_DisplayMode displaymode;
	SDL_GetCurrentDisplayMode(0, &displaymode);
	WindHeight = displaymode.h;
	WindWidth = displaymode.w;

	myWind = SDL_CreateWindow("G1_GAME_ENGINE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WindWidth, WindHeight,SDL_WINDOW_BORDERLESS);
	if (myWind == nullptr)
	{
		return;
	}
	myRen = SDL_CreateRenderer(myWind, 0, SDL_RENDERER_ACCELERATED);

	if (myRen == nullptr)
	{
		return;
	}

	myFont = TTF_OpenFont("font/Alan_Sans/static/AlanSans-Medium.ttf", 50);

	if (myFont == nullptr)
	{
		std::cout << "FONT NOT LOAD" << std::endl;
		return;
	}

	LivesFont = TTF_OpenFont("font/Alan_Sans/static/AlanSans-Medium.ttf", 50);
	if (LivesFont == nullptr)
	{
		std::cout << "FONT NOT LOAD" << std::endl;
		return;
	}

	GameOverFont = TTF_OpenFont("font/Alan_Sans/static/AlanSans-Medium.ttf", 50);
	if (GameOverFont == nullptr)
	{
		std::cout << "FONT NOT LOAD" << std::endl;
		return;
	}

	TopWallRect = { 0,0,WindWidth,thickness };

	BottomWallRect = { 0,WindHeight - thickness,WindWidth,thickness };

	LeftWallRect = { 0,0,thickness,WindHeight};

	RightWallRect = { WindWidth - thickness,0,thickness,WindHeight };

	PlayerPaddleRect = { static_cast<int>(PlayerPos.x),static_cast<int>(PlayerPos.y),thickness, length };

	EnemyPaddleRect = { static_cast<int>(EnemyPos.x),static_cast<int>(EnemyPos.y),thickness, length };
	
	BallRect = { static_cast<int>(BallPos.x - thickness/2),static_cast<int>(BallPos.y - thickness/2),thickness,thickness };

	ScoreFont = { 650,23,25,50 };
	FontLivesRect = { 500,20,150,50 };
	GameOverRect = { 780,450,300,150 };
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
	EnemyMovement();
	LoadFontOnScreen();
	
	if (Lives == 0)
	{
		GameOverTitle();
	}
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
	
	//EnemyPaddleRect
	SDL_RenderFillRect(myRen, &EnemyPaddleRect);

	//BallRect
	SDL_RenderFillRect(myRen, &BallRect);

	SDL_RenderCopy(myRen, myFontTexture, NULL, &ScoreFont);

	SDL_RenderCopy(myRen, myLivesTexture, NULL, &FontLivesRect);

	SDL_RenderCopy(myRen, GameOverTexture, NULL, &GameOverRect);

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
	SDL_DestroyTexture(myFontTexture);
	SDL_DestroyTexture(myLivesTexture);
	TTF_CloseFont(myFont);
	TTF_CloseFont(LivesFont);
	SDL_Quit();
}

void Game::PlayerMovement()
{

	if (Lives > 0)
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
			if (PlayerPos.y + 100 > WindHeight)
			{
				PlayerPos.y = WindHeight - 100;
			}
			PlayerPos.y += speed * deltatime;
			std::cout << "Moving Down :- PlayerPos Y = " << PlayerPos.y << std::endl;
		}
		PlayerPaddleRect.y = PlayerPos.y;
		//std::cout << "PlayerPaddleRect Y :- " << PlayerPaddleRect.y << std::endl;
	}
	

}
void Game::BallMovement()
{
	if (Lives > 0)
	{
		BallPos.x += BallVelX * deltatime;
		BallPos.y += BallVelY * deltatime;

		if (BallPos.x < 0)
		{
			BallPos.x = 0;
			BallVelX = -BallVelX;
			Lives -= 1;
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
}

void Game::EnemyMovement()
{

	if (Lives > 0)
	{
		float targetY = BallPos.y;

		//int RandomSpeedGenerate = (std::rand() % 550) + 1;

		//EnemyVelY = RandomSpeedGenerate;

		if (EnemyPos.y < targetY)
		{
			EnemyPos.y += EnemyVelY * deltatime;
		}
		else if (EnemyPos.y > targetY)
		{
			EnemyPos.y -= EnemyVelY * deltatime;
		}

		EnemyPaddleRect.y = EnemyPos.y;
	}

}

void Game::BallHitsPaddle()
{
	if (SDL_HasIntersection(&PlayerPaddleRect, &BallRect))
	{
		
		BallPos.x = PlayerPaddleRect.x + PlayerPaddleRect.w;

		BallVelX = -BallVelX;

		
		BallVelX += (BallVelX > 0 ? 1 : -1);
		BallVelY += (BallVelY > 0 ? 1 : -1);
	}

	
	if (SDL_HasIntersection(&EnemyPaddleRect, &BallRect))
	{
		
		BallPos.x = EnemyPaddleRect.x - thickness;

		BallVelX = -BallVelX;

		BallVelX += (BallVelX > 0 ? 1 : -1);
		BallVelY += (BallVelY > 0 ? 1 : -1);
	}

	
	BallRect.x = BallPos.x;
	BallRect.y = BallPos.y;

}

void Game::LoadFontOnScreen()
{
	std::string scoreText = std::to_string(Lives);
	std::string LivesText = " Player Lives :- ";

	myFontSurface = TTF_RenderText_Solid(myFont, scoreText.c_str(), { 255,255,255 });
	
	myLivesSurface = TTF_RenderText_Solid(LivesFont, LivesText.c_str(), { 255,255,255 });

		
	if (myFontTexture != nullptr)
	{
		SDL_DestroyTexture(myFontTexture);
		myFontTexture = nullptr;
	}

	if (myLivesTexture != nullptr)
	{
		SDL_DestroyTexture(myLivesTexture);
		myLivesTexture = nullptr;
	}

	myFontTexture = SDL_CreateTextureFromSurface(myRen, myFontSurface);

	myLivesTexture = SDL_CreateTextureFromSurface(myRen, myLivesSurface);

	SDL_FreeSurface(myFontSurface);
	SDL_FreeSurface(myLivesSurface);
}
void Game::GameOverTitle()
{
	std::string GameOver = "Game Over";

	GameOverSurface = TTF_RenderText_Solid(GameOverFont, GameOver.c_str(), { 255,255,255 });

	if (GameOverTexture != nullptr)
	{
		SDL_DestroyTexture(GameOverTexture);
		GameOverTexture = nullptr;
	}

	GameOverTexture = SDL_CreateTextureFromSurface(myRen, GameOverSurface);

	SDL_FreeSurface(GameOverSurface);
}