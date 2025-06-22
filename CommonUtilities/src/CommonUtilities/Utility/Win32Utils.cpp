#include <CommonUtilities/Utility/Win32Utils.h>

#include <CommonUtilities/System/WindowsHeader.h>
#include <algorithm>
#include <Psapi.h>
#include <Pdh.h>

using namespace CommonUtilities;

const Vector2f& CommonUtilities::GetDesktopResolution()
{
	static const auto desktopResolution = []
	{
		Vector2f result;

		DEVMODE win32Mode{};
		win32Mode.dmSize = sizeof(win32Mode);
		win32Mode.dmDriverExtra = 0;
		EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &win32Mode);

		result.x = (float)win32Mode.dmPelsWidth;
		result.y = (float)win32Mode.dmPelsHeight;

		return result;
	}();

	return desktopResolution;
}

float CommonUtilities::GetDesktopAspectRatio()
{
	Vector2f desktopResolution = GetDesktopResolution();
	return desktopResolution.x / desktopResolution.y;
}

const std::vector<Vector2f>& CommonUtilities::GetValidResolutions()
{
	static const auto validResolutions = []
	{
		std::vector<Vector2f> result;

		float desktopRatio = GetDesktopAspectRatio();

		DEVMODE win32Mode{};
		win32Mode.dmSize = sizeof(win32Mode);
		win32Mode.dmDriverExtra = 0;

		for (int count = 0; EnumDisplaySettings(nullptr, static_cast<DWORD>(count), &win32Mode); ++count)
		{
			Vector2f resolution 
			{ 
				(float)win32Mode.dmPelsWidth,
				(float)win32Mode.dmPelsHeight
			};

			float ratio = resolution.x / resolution.y;

			if (!Equal(ratio, desktopRatio))
				continue;

			if (std::find(result.begin(), result.end(), resolution) == result.end())
			{
				result.emplace_back(resolution);
			}
		}

		std::sort(result.begin(), result.end(), 
			[](const Vector2f& aLeft, const Vector2f& aRight)
			{
				if (aRight.y == aRight.y)
					return aLeft.x > aRight.x;

				return aLeft.y > aRight.y;
			});

		return result;
	}();

	return validResolutions;
}

long double CommonUtilities::Get_CPU_Usage()
{
	static const int numProcessors = 
		[] 
		{
			SYSTEM_INFO systemInfo;
			GetSystemInfo(&systemInfo);
			return systemInfo.dwNumberOfProcessors;
		}();

	static thread_local ULARGE_INTEGER lastSysCPU{};
	static thread_local ULARGE_INTEGER lastUserCPU{};
	static thread_local ULARGE_INTEGER lastRef{};

	FILETIME fileTime, fileSystem, fileUser;
	ULARGE_INTEGER now, sys, user;
	long double percent;

	GetSystemTimeAsFileTime(&fileTime);
	memcpy(&now, &fileTime, sizeof(FILETIME));

	if (now.QuadPart == lastRef.QuadPart)
		return 0.0;

	GetProcessTimes(GetCurrentProcess(), &fileTime, &fileTime, &fileSystem, &fileUser);
	memcpy(&sys, &fileSystem, sizeof(FILETIME));
	memcpy(&user, &fileUser, sizeof(FILETIME));

	percent = static_cast<long double>((sys.QuadPart - lastSysCPU.QuadPart) + (user.QuadPart - lastUserCPU.QuadPart));
	percent /= static_cast<long double>(now.QuadPart - lastRef.QuadPart);
	percent /= numProcessors;

	lastRef		= now;
	lastUserCPU = user;
	lastSysCPU	= sys;

	return percent;
}

std::size_t CommonUtilities::Get_RAM_Usage()
{
	PROCESS_MEMORY_COUNTERS_EX pmc{};
	GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));

	return (std::size_t)pmc.WorkingSetSize;
}
