#include "AnimSpriteComponent.h"

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder):
	SpriteComponent(owner, drawOrder),
	curFrame(0.0f),
	animFPS(24.0f)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (animTextures.size() > 0)
	{
		curFrame += animFPS * deltaTime;

		while (curFrame >= animTextures.size())
		{
			curFrame -= animTextures.size();
		}

		SetTexture(animTextures[static_cast<int>(curFrame)]);
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	animTextures = textures;
	if (animTextures.size() > 0)
	{
		curFrame = 0.0f;
		SetTexture(animTextures[0]);
	}
}