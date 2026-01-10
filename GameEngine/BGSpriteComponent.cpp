#include "BGSpriteComponent.h"

BGSpriteComponent::BGSpriteComponent(Actor* owner, int drawOrder):
	SpriteComponent(owner, drawOrder),
	scrollSpeed(0.0f)
{
}

void BGSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	for (auto& texture : textures)
	{
		texture->offset.x += scrollSpeed * deltaTime;

		if (texture->offset.x < -screenSize.x)
		{
			texture->offset.x = (textures.size() - 1) * screenSize.x - 1;
		}
	}
}

void BGSpriteComponent::Draw(SDL_Renderer* renderer)
{
}

void BGSpriteComponent::SetBGTexture(const std::vector<SDL_Texture*> textures)
{
	int count = 0;
	
	for (auto texture : textures)
	{
		BGTexture tmp;
		tmp.texture = texture;
		tmp.offset.x = count * screenSize.x;
		tmp.offset.y = 0;
		
		this->textures.emplace_back(tmp);
		count++;
	}
}

