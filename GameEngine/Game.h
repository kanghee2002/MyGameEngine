#pragma once

#include <SDL.h>

struct Vector2
{
	float x;
	float y;
};

class Game
{
public:
	Game();

	bool Initialize();

	void RunLoop();

	void Shutdown();

private:
	void ProcessInput();

	void UpdateGame();

	void GenerateOutput();

	SDL_Window* window;

	SDL_Renderer* renderer;

	bool isRunning;

	Uint32 ticksCount;

	Vector2 paddlePos;

	Vector2 ballPos;
};