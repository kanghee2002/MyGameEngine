#pragma once

#include "SpriteComponent.h"
#include <vector>

class AnimSpriteComponent : public SpriteComponent
{
public:
	AnimSpriteComponent(class Actor* owner, int drawOrder = 100);
	
	void Update(float deltaTime) override;
	
	void SetAnimTextures(const std::vector<SDL_Texture*>& textures);

	float GetAnimFPS() const { return animFPS; }
	void SetFPS(float fps) { animFPS = fps; }

private:
	std::vector<SDL_Texture*> animTextures;

	float curFrame;
	float animFPS;
};