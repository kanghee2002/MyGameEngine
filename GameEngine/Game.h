#pragma once

#include <SDL.h>
#include "Math.h"
#include <vector>

class Game
{
public:
	Game();

	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor* actor);
	void RemoveActor(class Actor* actor);

	void AddSprite(class SpriteComponent* sprite);
	void RemoveSprite(class SpriteComponent* sprite);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();

	std::vector<class Actor*> actors;
	std::vector<class Actor*> pendingActors;

	std::vector<class SpriteComponent*> sprites;

	SDL_Window* window;
	SDL_Renderer* renderer;

	Uint32 ticksCount;

	bool isRunning;
	bool isUpdatingActors;

	int paddleDir;

	Vector2 paddlePos;

	Vector2 ballPos;

	Vector2 ballVelocity;
};