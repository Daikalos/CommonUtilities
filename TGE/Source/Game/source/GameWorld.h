#pragma once

#include <CommonUtilities/Input/InputHolder.h>

class GameWorld
{
public:
	GameWorld(); 
	~GameWorld();

	void Init();
	void Update(cu::InputHolder& aInputHandler, float aTimeDelta);
	void Render();

private:

};