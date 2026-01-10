#pragma once

#include <vector>
#include "Math.h"

class Actor
{
public:
	enum State
	{
		EActvie,
		EPaused,
		EDead
	};

	Actor(class Game* game);

	virtual ~Actor();

	void Update(float deltaTime);

	void UpdateComponents(float deltaTime);

	virtual void UpdateActor(float deltaTime);

	State GetState() const { return state; }
	void SetState(State state) { this->state = state; }

	const Vector2& GetPosition() const { return position; }
	void SetPosition(const Vector2& position) { this->position = position; }

	float GetRotation() const { return rotation; }
	void SetRotation(float rotation) { this->rotation = rotation; }

	float GetScale() const { return scale; }
	void SetScale(float scale) { this->scale = scale; }

	void AddComponent(class Component* component);

	void RemoveComponent(class Component* component);

private:
	State state;

	Vector2 position;

	float rotation;

	float scale;

	std::vector<class Component*> components;

	class Game* game;
};