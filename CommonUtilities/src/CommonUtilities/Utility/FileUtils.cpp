#include <CommonUtilities/Utility/FileUtils.h>

#include <fstream>

namespace fs = std::filesystem;

std::size_t CommonUtilities::FileSize(const std::filesystem::path& aPath)
{
	if (fs::exists(aPath) && fs::is_regular_file(aPath))
		return fs::file_size(aPath);

	return 0;
}

std::vector<std::byte> CommonUtilities::ReadFileBytes(const std::filesystem::path& aPath)
{
	std::vector<std::byte> data;

	std::ifstream fs;
	fs.open(aPath, std::ifstream::in | std::ifstream::binary);

	if (fs.good())
	{
		if (auto size = FileSize(aPath); size != 0)
		{
			data.resize(size);
			fs.seekg(0, std::ios::beg);
			fs.read(reinterpret_cast<char*>(data.data()), size);
			fs.close();
		}
	}

	return data;
}
