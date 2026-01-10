#include "SDL.h"
#include "SDL_image.h"

#include "Game.h"
#include "Actor.h"
#include "SpriteComponent.h"

const float screenHeight = 768.0f;
const float screenWidth = 1080.0f;

const int thickness = 15;
const float paddleHeight = 100.0f;

Game::Game() :
	window(nullptr),
	renderer(nullptr),
	isRunning(true),
	isUpdatingActors(false)
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
		screenWidth, screenHeight,
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

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	paddlePos.x = 10.0f;
	paddlePos.y = screenHeight / 2;
	ballPos.x = screenWidth / 2;
	ballPos.y = screenHeight / 2;
	ballVelocity.x = -200.0f / 2;
	ballVelocity.y = 235.0f / 2;

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

void Game::AddActor(Actor* actor)
{
	if (isUpdatingActors)
	{
		pendingActors.emplace_back(actor);
	}
	else
	{
		actors.emplace_back(actor);
	}
}

void Game::RemoveActor(Actor* actor)
{
}

void Game::AddSprite(SpriteComponent* sprite)
{
	int drawOrder = sprite->GetDrawOrder();

	auto iter = sprites.begin();
	for (; iter != sprites.end(); iter++)
	{
		if (drawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	sprites.insert(iter, sprite);
}

void Game::RemoveSprite(SpriteComponent* sprite)
{
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

		paddleDir = 0;
		if (state[SDL_SCANCODE_W])
		{
			paddleDir -= 1;
		}
		if (state[SDL_SCANCODE_S])
		{
			paddleDir += 1;
		}
	}
}

void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), ticksCount + 16));

	float deltaTime = SDL_GetTicks() - ticksCount;
	deltaTime /= 100.0f;	// Convert to seconds

	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	ticksCount = SDL_GetTicks();

	isUpdatingActors = true;
	for (auto actor : actors)
	{
		actor->Update(deltaTime);
	}
	isUpdatingActors = false;

	for (auto pendingActor : pendingActors)
	{
		actors.emplace_back(pendingActor);
	}
	pendingActors.clear();

	std::vector<Actor*> deadActors;
	for (auto actor : actors)
	{
		if (actor->GetState() == Actor::EDead)
		{
			deadActors.emplace_back(actor);
		}
	}

	for (auto actor : deadActors)
	{
		delete actor;
	}

	if (paddleDir != 0)
	{
		paddlePos.y += paddleDir * 300.0f * deltaTime;

		if (paddlePos.y < (paddleHeight / 2.0f + thickness))
		{
			paddlePos.y = paddleHeight / 2.0f + thickness;
		}
		else if (paddlePos.y > screenHeight - (paddleHeight / 2.0f + thickness))
		{
			paddlePos.y = screenHeight - (paddleHeight / 2.0f + thickness);
		}
	}

	ballPos.x += ballVelocity.x * deltaTime;
	ballPos.y += ballVelocity.y * deltaTime;


	// Collide Top Wall
	if (ballPos.y <= thickness && ballVelocity.y < 0.0f)
	{
		ballVelocity.y *= -1;
	}

	// Collide Bottom Wall
	if (ballPos.y >= screenHeight - thickness && ballVelocity.y > 0.0f)
	{
		ballVelocity.y *= -1;
	}

	// Collide Right Wall
	if (ballPos.x >= screenWidth - thickness && ballVelocity.x > 0.0f)
	{
		ballVelocity.x *= -1;
	}

	// Collide Paddle 
	if (ballPos.x >= paddlePos.x && ballPos.x <= paddlePos.x + thickness)
	{
		float yDiff = paddlePos.y - ballPos.y;
		if (yDiff < 0.0f) yDiff = -yDiff;

		if (yDiff <= paddleHeight / 2 && ballVelocity.x < 0.0f)
		{
			ballVelocity.x *= -1;
		}
	}
}

void Game::GenerateOutput()
{
	// Draw Background
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 128);

	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 128);

	// Draw Wall
	SDL_Rect topWall{ 0, 0, screenWidth, thickness };
	SDL_Rect bottomWall{ 0, screenHeight - thickness, screenWidth, thickness };
	SDL_Rect leftWall{ 0, 0, thickness, screenHeight };
	SDL_Rect rightWall{ screenWidth - thickness, 0, thickness, screenWidth };

	SDL_RenderFillRect(renderer, &topWall);
	SDL_RenderFillRect(renderer, &bottomWall);
	SDL_RenderFillRect(renderer, &rightWall);
	
	// Draw Paddle
	SDL_Rect paddle{ 
		static_cast<int>(paddlePos.x),
		static_cast<int>(paddlePos.y - (paddleHeight / 2)),
		thickness,
		paddleHeight
	};
	SDL_RenderFillRect(renderer, &paddle);

	// Draw Ball
	SDL_Rect ball{
		static_cast<int>(ballPos.x - thickness / 2),
		static_cast<int>(ballPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(renderer, &ball);




	SDL_RenderPresent(renderer);
}
