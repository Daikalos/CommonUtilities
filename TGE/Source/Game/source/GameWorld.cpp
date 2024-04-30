#include "stdafx.h"

#include <CommonUtilities/Math/Matrix.hpp>
#include <CommonUtilities/Utility/PrintUtils.hpp>

#include "GameWorld.h"
#include <tge/graphics/GraphicsEngine.h>
#include <tge/drawers/SpriteDrawer.h>
#include <tge/texture/TextureManager.h>
#include <tge/drawers/DebugDrawer.h>

GameWorld::GameWorld()
{

}

GameWorld::~GameWorld() 
{

}

void GameWorld::Init()  
{

}
void GameWorld::Update(UNSD cu::InputHolder& aInputHandler, [[maybe_unused]] float aTimeDelta)
{

}

void GameWorld::Render()
{
	auto &engine = *Tga::Engine::GetInstance();
	[[maybe_unused]] Tga::SpriteDrawer& spriteDrawer = engine.GetGraphicsEngine().GetSpriteDrawer();


}