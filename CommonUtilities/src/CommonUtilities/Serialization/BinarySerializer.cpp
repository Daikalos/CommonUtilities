#include <CommonUtilities/Serialization/BinarySerializer.h>

using namespace CommonUtilities;

BinarySerializer::BinarySerializer(SerializerState aState)
	: myState(aState)
	, myBuffer()
	, myOffset(0)
{

}

BinaryReadSerializer::BinaryReadSerializer(std::span<const std::byte> aBuffer)
	: BinarySerializer(SerializerState::Read)
{
	myBuffer = { aBuffer.begin(), aBuffer.end() };
}
BinaryReadSerializer::BinaryReadSerializer(std::vector<std::byte>&& aBuffer)
	: BinarySerializer(SerializerState::Read)
{
	myBuffer = std::move(aBuffer);
}

BinaryWriteSerializer::BinaryWriteSerializer()
	: BinarySerializer(SerializerState::Write)
{

}

void BinaryWriteSerializer::ReserveBytesToFit(std::size_t aNumBytesToFit)
{
	myBuffer.reserve(myBuffer.size() + aNumBytesToFit);
}
void BinaryWriteSerializer::FitBufferToOffset()
{
	myBuffer.resize(myOffset);
	myBuffer.shrink_to_fit();
}

void BinaryWriteSerializer::Clear()
{
	myBuffer.clear();
	myOffset = 0;
}

std::size_t SerializeAsBinary<std::string>::operator()(SerializerState aState, std::string& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
{
	if (aState == SerializerState::Read)
	{
		aInOutData = reinterpret_cast<const char*>(aInOutBytes.data() + aOffset);
	}
	else
	{
		aInOutBytes.resize(aOffset + aInOutData.length() + 1);
		memcpy_s(aInOutBytes.data() + aOffset, aInOutData.length() + 1, aInOutData.c_str(), aInOutData.length() + 1);
		assert(strlen(reinterpret_cast<const char*>(aInOutBytes.data() + aOffset)) == aInOutData.length()); // make sure it went well
	}

	return aInOutData.length() + 1;
}
std::size_t SerializeAsBinary<std::string>::operator()(SerializerState aState, const std::string& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
{
	if (aState == SerializerState::Read)
	{
		assert(false && "Cannot copy to const memory!");
	}
	else
	{
		aInOutBytes.resize(aOffset + aInOutData.length() + 1);
		memcpy_s(aInOutBytes.data() + aOffset, aInOutData.length() + 1, aInOutData.c_str(), aInOutData.length() + 1);
		assert(strlen(reinterpret_cast<const char*>(aInOutBytes.data() + aOffset)) == aInOutData.length()); // make sure it went well
	}

	return aInOutData.length() + 1;
}

std::size_t SerializeAsBinary<std::wstring>::operator()(SerializerState aState, std::wstring& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
{
	static constexpr std::size_t WCHAR_SIZE = sizeof(wchar_t);

	if (aState == SerializerState::Read)
	{
		aInOutData = reinterpret_cast<const wchar_t*>(aInOutBytes.data() + aOffset);
	}
	else
	{
		aInOutBytes.resize(aOffset + (aInOutData.length() + 1) * WCHAR_SIZE);
		memcpy_s(aInOutBytes.data() + aOffset, (aInOutData.length() + 1) * WCHAR_SIZE, aInOutData.c_str(), (aInOutData.length() + 1) * WCHAR_SIZE);
		assert(wcslen(reinterpret_cast<const wchar_t*>(aInOutBytes.data() + aOffset)) == aInOutData.length()); // make sure it went well
	}

	return (aInOutData.length() + 1) * WCHAR_SIZE;
}
std::size_t SerializeAsBinary<std::wstring>::operator()(SerializerState aState, const std::wstring& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
{
	static constexpr std::size_t WCHAR_SIZE = sizeof(wchar_t);

	if (aState == SerializerState::Read)
	{
		assert(false && "Cannot copy to const memory!");
	}
	else
	{
		aInOutBytes.resize(aOffset + (aInOutData.length() + 1) * WCHAR_SIZE);
		memcpy_s(aInOutBytes.data() + aOffset, (aInOutData.length() + 1) * WCHAR_SIZE, aInOutData.c_str(), (aInOutData.length() + 1) * WCHAR_SIZE);
		assert(wcslen(reinterpret_cast<const wchar_t*>(aInOutBytes.data() + aOffset)) == aInOutData.length()); // make sure it went well
	}

	return (aInOutData.length() + 1) * WCHAR_SIZE;
}