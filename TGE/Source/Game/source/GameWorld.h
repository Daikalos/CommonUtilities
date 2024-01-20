#pragma once

#include <CommonUtilities/Input/InputHolder.h>
#include <CommonUtilities/Input/InputBind.hpp>

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
};