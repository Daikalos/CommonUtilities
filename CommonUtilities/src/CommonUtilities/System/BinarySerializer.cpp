#include <CommonUtilities\System\BinarySerializer.h>

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
