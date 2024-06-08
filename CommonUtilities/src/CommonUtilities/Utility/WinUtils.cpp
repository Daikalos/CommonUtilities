#include <CommonUtilities\Utility\WinUtils.h>

#include <Psapi.h>
#include <Pdh.h>

const cu::Vector2f& CommonUtilities::GetDesktopResolution()
{
	static const auto desktopResolution = []
	{
		cu::Vector2f result;

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

const std::vector<cu::Vector2f>& CommonUtilities::GetValidResolutions()
{
	static const auto validResolutions = []
	{
		std::vector<cu::Vector2f> result;

		cu::Vector2f desktopResolution = GetDesktopResolution();
		float desktopRatio = desktopResolution.x / desktopResolution.y;

		DEVMODE win32Mode{};
		win32Mode.dmSize = sizeof(win32Mode);
		win32Mode.dmDriverExtra = 0;

		for (int count = 0; EnumDisplaySettings(nullptr, static_cast<DWORD>(count), &win32Mode); ++count)
		{
			cu::Vector2f resolution 
			{ 
				(float)win32Mode.dmPelsWidth,
				(float)win32Mode.dmPelsHeight
			};

			float ratio = resolution.x / resolution.y;

			if (!cu::au::Equal(ratio, desktopRatio))
				continue;

			if (std::find(result.begin(), result.end(), resolution) == result.end())
			{
				result.emplace_back(resolution);
			}
		}

		std::sort(result.begin(), result.end(), 
			[](const cu::Vector2f& aLeft, const cu::Vector2f& aRight)
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

	lastRef = now;
	lastUserCPU = user;
	lastSysCPU	= sys;

	return percent;
}