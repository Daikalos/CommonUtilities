#pragma once

#include <tge/sprite/sprite.h>

#include <CommonUtilities/Input/InputHolder.h>
#include <CommonUtilities/Input/InputBind.hpp>
#include <CommonUtilities/Input/GamepadInput.h>

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
	cu::InputBind<GameActions> myInputBind;
	Tga::Sprite3DInstanceData mySpriteInstance;
	Tga::SpriteSharedData mySpriteSharedData;
};