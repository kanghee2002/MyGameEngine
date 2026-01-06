#include "Game.h"

const int thickness = 15;

Game::Game() :
	window(nullptr),
	renderer(nullptr),
	isRunning(true),
	ticksCount(0)
{

}

bool Game::Initialize()
{
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);

	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());

		return false;
	}

	window = SDL_CreateWindow(
		"Physics Engine",
		100, 100,
		1024, 768,
		0
	);

	if (!window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());

		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());

		return false;
	}

	return true;
}

void Game::RunLoop()
{
	while (isRunning)
	{
		ProcessInput();
		UpdateGame();
		GenerateOutput();
	}
}

void Game::Shutdown()
{
	SDL_DestroyWindow(window);

	SDL_DestroyRenderer(renderer);

	SDL_Quit();
}

void Game::ProcessInput()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			isRunning = false;
			break;

		default:
			break;
		}

		const Uint8* state = SDL_GetKeyboardState(NULL);

		if (state[SDL_SCANCODE_ESCAPE])
		{
			isRunning = false;
		}
	}
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

	float deltaTime = SDL_GetTicks() - ticksCount;
	deltaTime /= 100.0f;	// Convert to seconds

	ticksCount = SDL_GetTicks();

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}

}

void Game::GenerateOutput()
{
	// Draw Background
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 128);

	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);

	// Draw Wall
	SDL_Rect topWall{ 0, 0, 1024, thickness };
	SDL_Rect bottomWall{ 0, 768 - thickness, 1024, thickness };
	SDL_Rect leftWall{ 0, 0, thickness, 768 };
	SDL_Rect rightWall{ 1024 - thickness, 0, thickness, 1024 };

	SDL_RenderFillRect(renderer, &topWall);
	SDL_RenderFillRect(renderer, &bottomWall);
	SDL_RenderFillRect(renderer, &leftWall);
	SDL_RenderFillRect(renderer, &rightWall);
	
	// Draw Ball
	SDL_Rect ball{
		static_cast<int>(ballPos.x - thickness / 2),
		static_cast<int>(ballPos.y - thickness / 2),
		thickness,
		thickness
	};





	SDL_RenderPresent(renderer);
}
