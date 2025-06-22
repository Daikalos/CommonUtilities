#include <CommonUtilities/System/IDGenerator.h>

#include <combaseapi.h>
#include <sstream>
#include <string>
#include <iomanip>

namespace CommonUtilities
{
	std::string CreateGUID()
	{
		std::string guidString;

		GUID guid;
		HRESULT hr = CoCreateGuid(&guid);

		if (SUCCEEDED(hr))
		{
			std::stringstream stream;

			stream << std::hex << std::uppercase
				<< std::setw(8) << std::setfill('0') << guid.Data1
				<< "-" << std::setw(4) << std::setfill('0') << guid.Data2
				<< "-" << std::setw(4) << std::setfill('0') << guid.Data3
				<< "-";

			for (int i = 0; i < sizeof(guid.Data4); ++i)
			{
				if (i == 2)
					stream << "-";

				stream << std::hex << std::setw(2) << std::setfill('0') << (int)guid.Data4[i];
			}

			guidString = stream.str();
		}

		return guidString;
	}
}
