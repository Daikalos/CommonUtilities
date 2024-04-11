#pragma once

#include <vector>
#include <memory>
#include <span>
#include <cassert>

#include <CommonUtilities\Config.h>

namespace CommonUtilities
{ 
	enum class SerializerState
	{
		Read,
		Write
	};

	template<typename T> requires (std::is_trivially_copyable_v<T>) // trivially copyable is required to prevent UB
	struct SerializeAsBinary
	{
		NODISC std::size_t operator()(SerializerState aState, std::size_t aOffset, T& aInOutData, std::vector<std::byte>& aInOutBytes);
	};

	class BinarySerializer
	{
	public:
		BinarySerializer() = delete;
		virtual ~BinarySerializer() = default;

		NODISC std::size_t GetOffset() const noexcept { return myOffset; }

		template<typename... Ts> requires (std::is_trivially_copyable_v<Ts> && ...)
		void Serialize(Ts&... aInOutData);

	protected:
		COMMON_UTILITIES_API BinarySerializer(SerializerState aState);

		SerializerState			myState;
		std::vector<std::byte>	myBuffer;
		std::size_t				myOffset;
	};

	class BinaryReadSerializer : public BinarySerializer
	{
	public:
		COMMON_UTILITIES_API BinaryReadSerializer(std::span<const std::byte> aBuffer);
	};

	class BinaryWriteSerializer : public BinarySerializer
	{
	public:
		COMMON_UTILITIES_API BinaryWriteSerializer();

		NODISC std::span<const std::byte> GetBuffer() const noexcept { return myBuffer; }
		NODISC const std::byte* GetBufferData() const noexcept { return myBuffer.data(); }

		void ReserveBytesToFit(std::size_t aNumBytesToFit);
		void FitBufferToOffset();
	};

	template<typename... Ts> requires (std::is_trivially_copyable_v<Ts> && ...)
	inline void BinarySerializer::Serialize(Ts&... aInOutData)
	{
		((myOffset += SerializeAsBinary<Ts>{}(myState, myOffset, aInOutData, myBuffer)), ...);
	}

	template<typename T> requires (std::is_trivially_copyable_v<T>)
	inline std::size_t SerializeAsBinary<T>::operator()(SerializerState aState, std::size_t aOffset, T& aInOutData, std::vector<std::byte>& aInOutBytes)
	{
		static constexpr std::size_t numBytes = sizeof(T);

		if (aState == SerializerState::Read)
		{
			assert((aOffset + numBytes) <= aInOutBytes.size());
			memcpy_s(&aInOutData, numBytes, aInOutBytes.data() + aOffset, numBytes);
		}
		else
		{
			aInOutBytes.resize(aOffset + numBytes);
			memcpy_s(aInOutBytes.data() + aOffset, numBytes, &aInOutData, numBytes);
		}

		return numBytes;
	}
}