#include "SpriteComponent.h"
#include "Actor.h"
#include "Game.h"

SpriteComponent::SpriteComponent(Actor* owner, int drawOrder):
	Component(owner),
	texture(nullptr),
	drawOrder(drawOrder),
	textureWidth(0),
	textureHeight(0)
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (texture)
	{
		SDL_Rect rect;

		rect.w = static_cast<int>(textureWidth * owner->GetScale());
		rect.h = static_cast<int>(textureHeight * owner->GetScale());
		rect.x = static_cast<int>(owner->GetPosition().x - rect.w / 2);
		rect.y = static_cast<int>(owner->GetPosition().y - rect.h / 2);
		
		SDL_RenderCopyEx(renderer, texture, nullptr, &rect,
			-Math::ToDegrees(owner->GetRotation()), nullptr, SDL_FLIP_NONE);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	this->texture = texture;

	SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
}
