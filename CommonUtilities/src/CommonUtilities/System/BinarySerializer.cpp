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
