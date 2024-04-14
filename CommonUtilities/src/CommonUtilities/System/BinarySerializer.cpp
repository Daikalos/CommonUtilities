#include <CommonUtilities\System\BinarySerializer.h>

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
	myBuffer.resize(aBuffer.size());
	memcpy_s(myBuffer.data(), myBuffer.size(), aBuffer.data(), myBuffer.size());
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