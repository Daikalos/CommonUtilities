#include <CommonUtilities\Utility\WinUtils.h>

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
