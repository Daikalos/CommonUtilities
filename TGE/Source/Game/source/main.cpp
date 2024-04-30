#include "stdafx.h"
#include "GameWorld.h"

#include <CommonUtilities/Input/InputHandler.h>
#include <CommonUtilities/System/Timer.h>

#include <tge/math/Matrix4x4.h>
#include <CommonUtilities/Math/Matrix4x4.hpp>

cu::InputHolder globalInputHolder;

void Go(void);

#if _DEBUG || _RELEASE
int main(const int /*argc*/, const char* /*argc*/[])
{
	Go();
	return 0;
}
#elif _RETAIL
int APIENTRY wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPTSTR, _In_ int)
{
	Go();
	return 0;
}
#else
#error Unknown build.
#endif

LRESULT WinProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	lParam;
	wParam;
	hWnd;

	if (globalInputHolder.HandleEvent(message, wParam, lParam))
	{
		return 0;
	}

	switch (message)
	{
		// this message is read when the window is closed
		case WM_DESTROY:
		{
			// close the application entirely
			PostQuitMessage(0);
			return 0;
		}
	}

	return 0;
}


void Go()
{
	Tga::LoadSettings(TGE_PROJECT_SETTINGS_FILE);

	Tga::EngineConfiguration winconf;
	
	winconf.myApplicationName = L"TGE - Amazing Game";
	winconf.myWinProcCallback = [](HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) { return WinProc(hWnd, message, wParam, lParam); };
#ifdef _DEBUG
	winconf.myActivateDebugSystems = Tga::DebugFeature::Fps | Tga::DebugFeature::Mem | Tga::DebugFeature::Filewatcher | Tga::DebugFeature::Cpu | Tga::DebugFeature::Drawcalls | Tga::DebugFeature::OptimizeWarnings;
#else
	winconf.myActivateDebugSystems = Tga::DebugFeature::Filewatcher;
#endif

	if (!Tga::Engine::Start(winconf))
	{
		ERROR_PRINT("Fatal error! Engine could not start!");
		system("pause");
		return;
	}

	cu::Vector3f rot{ -0.5f, 0.2f, 1.0f };

	cu::Mat4f cuMat;
	cuMat.SetRotation(rot);

	Tga::Vector3f tgaRot = (Tga::Vector3f)(rot * cu::au::RAD2DEG);
	//tgaRot = (tgaRot.z, tgaRot.y, tgaRot.x);

	Tga::Matrix4x4f tgaMat;
	tgaMat.SetRotation(tgaRot);

	std::cin.get();

	{
		cu::Timer timer;

		GameWorld gameWorld;
		gameWorld.Init();

		Tga::Engine& engine = *Tga::Engine::GetInstance();

		globalInputHolder.Cursor().Connect(*engine.GetHWND());

		while (engine.BeginFrame())
		{
			timer.Update();
			globalInputHolder.Update();

			gameWorld.Update(globalInputHolder, timer.GetDT());
			gameWorld.Render();

			engine.EndFrame();
		}
	}

	Tga::Engine::GetInstance()->Shutdown();
}

