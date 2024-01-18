#include "stdafx.h"

#include <CommonUtilities/Math/Matrix.hpp>

#include "GameWorld.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>

#include "EnumKeys.h"

GameWorld::GameWorld()
{

}

GameWorld::~GameWorld() 
{

}

void GameWorld::Init()  
{
	[[maybe_unused]] auto& engine = *Tga::Engine::GetInstance();
}
void GameWorld::Update(cu::InputHolder& aInputHandler, [[maybe_unused]] float aTimeDelta)
{
	if (aInputHandler.Keyboard().IsHeld(cu::Keyboard::W))
	{
		cu::Vector2i pos = aInputHandler.Cursor().GetMouseDelta();
		std::cout << pos.x << " " << pos.y << '\n';
	}
}

void GameWorld::Render()
{
	auto &engine = *Tga::Engine::GetInstance();
	[[maybe_unused]] Tga::SpriteDrawer& spriteDrawer = engine.GetGraphicsEngine().GetSpriteDrawer();

}