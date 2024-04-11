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

	template<typename T, SerializerState> requires (std::is_trivially_copyable_v<T>) // trivially copyable is required to prevent UB
	struct SerializeAsBinary
	{
		
	};

	template<typename T> requires (std::is_trivially_copyable_v<T>)
	struct SerializeAsBinary<T, SerializerState::Read>
	{
		NODISC std::size_t operator()(std::size_t aOffset, T& aOutData, const std::vector<std::byte>& aInBytes);
	};

	template<typename T> requires (std::is_trivially_copyable_v<T>)
	struct SerializeAsBinary<T, SerializerState::Write>
	{
		NODISC std::size_t operator()(std::size_t aOffset, const T& aInData, std::vector<std::byte>& aOutBytes);
	};

	class BinarySerializer
	{
	public:
		BinarySerializer() = delete;
		virtual ~BinarySerializer() = default;

		NODISC std::size_t GetOffset() const noexcept { return myOffset; }

		template<typename... Ts> requires (std::is_trivially_copyable_v<std::remove_reference_t<Ts>> && ...)
		void Serialize(Ts&&... aInOutData);

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

	template<typename... Ts> requires (std::is_trivially_copyable_v<std::remove_reference_t<Ts>> && ...)
	inline void BinarySerializer::Serialize(Ts&&... aInOutData)
	{
		((myOffset += SerializeAsBinary<std::remove_reference_t<Ts>>{}(myOffset, aInOutData, myBuffer)), ...);
	}

	template<typename T> requires (std::is_trivially_copyable_v<T>)
	inline std::size_t SerializeAsBinary<T, SerializerState::Read>::operator()(std::size_t aOffset, T& aOutData, const std::vector<std::byte>& aInBytes)
	{
		static constexpr std::size_t numBytes = sizeof(T);

		assert((aOffset + numBytes) <= aInBytes.size());
		memcpy_s(&aOutData, numBytes, aInBytes.data() + aOffset, numBytes);

		return numBytes;
	}

	template<typename T> requires (std::is_trivially_copyable_v<T>)
	inline std::size_t SerializeAsBinary<T, SerializerState::Write>::operator()(std::size_t aOffset, const T& aInData, std::vector<std::byte>& aOutBytes)
	{
		static constexpr std::size_t numBytes = sizeof(T);

		aOutBytes.resize(aOffset + numBytes);
		memcpy_s(aInData.data() + aOffset, numBytes, &aOutBytes, numBytes);

		return numBytes;
	}
}