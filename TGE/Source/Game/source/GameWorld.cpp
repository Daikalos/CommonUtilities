#include "stdafx.h"

#include <CommonUtilities/Math/Matrix.hpp>
#include <CommonUtilities/Utility/PrintUtils.hpp>

#include "GameWorld.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>

#include "EnumKeys.h"

GameWorld::GameWorld() : myInputBind()
{
	myInputBind.Set(GameActions::Up, cu::Mouse::Left);
	myInputBind.Set(GameActions::Up, cu::Keyboard::E);
}

GameWorld::~GameWorld() 
{

}

void GameWorld::Init()  
{
	[[maybe_unused]] auto& engine = *Tga::Engine::GetInstance();
	mySpriteSharedData.myTexture = engine.GetTextureManager().GetTexture(L"Textures/Helicopter.png");

	cu::Mat4f matrix = cu::Mat4f::CreateTRS(
		cu::Vector3f(512.0f, 512.0f, 0.0f), 
		cu::Vector3f(0.0f, 0.0f, 0.0f), 
		cu::Vector3f(100.0f, 100.0f, 1.0f));

	for (int i = 0; i < 16; ++i)
	{
		mySpriteInstance.myTransform.GetDataPtr()[i] = matrix[i];
	}

}
void GameWorld::Update(cu::InputHolder& aInputHandler, [[maybe_unused]] float aTimeDelta)
{
	aInputHandler.Gamepad().Connect();

	myInputBind.Connect(aInputHandler.Keyboard());
	myInputBind.Connect(aInputHandler.Mouse());

	if (myInputBind.IsHeld(GameActions::Up))
	{
		cu::Vector2i pos = aInputHandler.Cursor().GetMouseDelta();
		std::cout << pos.x << " " << pos.y << '\n';
	}
	if (aInputHandler.Gamepad().IsHeld(cu::Gamepad::A))
	{
		std::cout << aInputHandler.Gamepad().GetLeftStick() << '\n';
	}
}

void GameWorld::Render()
{
	auto &engine = *Tga::Engine::GetInstance();
	[[maybe_unused]] Tga::SpriteDrawer& spriteDrawer = engine.GetGraphicsEngine().GetSpriteDrawer();

	spriteDrawer.Draw(mySpriteSharedData, mySpriteInstance);
}