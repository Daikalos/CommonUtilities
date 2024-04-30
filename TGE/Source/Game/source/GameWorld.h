#pragma once

#include <tge/sprite/sprite.h>

#include <CommonUtilities/Input/InputHolder.h>

enum class GameActions
{
	Up,
	Left,
	Down,
	Right
};

class GameWorld
{
public:
	GameWorld(); 
	~GameWorld();

	void Init();
	void Update(cu::InputHolder& aInputHandler, float aTimeDelta);
	void Render();

private:
	Tga::Sprite3DInstanceData mySpriteInstance;
	Tga::SpriteSharedData mySpriteSharedData;
};