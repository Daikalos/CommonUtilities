#pragma once

#include <vector>
#include <string>
#include <vector>
#include <memory>
#include <span>
#include <cassert>

#include <CommonUtilities\Utility\Traits.h>
#include <CommonUtilities\Config.h>

namespace CommonUtilities
{ 
	enum class SerializerState
	{
		Read,
		Write
	};

	template<typename T>
	struct SerializeAsBinary
	{
		NODISC std::size_t operator()(SerializerState aState, T& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (std::is_trivially_copyable_v<T>); // trivially copyable is required to prevent UB
	};

	template<>
	struct SerializeAsBinary<std::string>
	{
		NODISC std::size_t operator()(SerializerState aState, std::string& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename T> requires (std::is_trivially_copyable_v<T>)
	struct SerializeAsBinary<std::vector<T>>
	{
		NODISC std::size_t operator()(SerializerState aState, std::vector<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	class BinarySerializer
	{
	public:
		BinarySerializer() = delete;
		virtual ~BinarySerializer() = default;

		NODISC std::size_t GetOffset() const noexcept { return myOffset; }

		template<typename... Ts> requires (std::is_trivially_copyable_v<std::decay_t<Ts>> && ...) && (sizeof...(Ts) > 1)
		void Serialize(Ts&... aInOutData);

		template<typename T> requires (std::is_trivially_copyable_v<std::decay_t<T>>)
		void Serialize(T& aInOutData);

		template<typename T> requires (std::is_trivially_copyable_v<std::decay_t<T>>)
		void Serialize(const T& aInOutData);

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

		COMMON_UTILITIES_API void ReserveBytesToFit(std::size_t aNumBytesToFit);
		COMMON_UTILITIES_API void FitBufferToOffset();
	};

	template<typename... Ts> requires (std::is_trivially_copyable_v<std::decay_t<Ts>> && ...) && (sizeof...(Ts) > 1)
	inline void BinarySerializer::Serialize(Ts&... aInOutData)
	{
		(Serialize(aInOutData), ...);
	}

	template<typename T> requires (std::is_trivially_copyable_v<std::decay_t<T>>)
	inline void BinarySerializer::Serialize(T& aInOutData)
	{
		myOffset += SerializeAsBinary<T>{}(myState, aInOutData, myBuffer, myOffset);
	}

	template<typename T> requires (std::is_trivially_copyable_v<std::decay_t<T>>)
	inline void BinarySerializer::Serialize(const T& aInOutData)
	{
		assert(myState == SerializerState::Write && "Cannot copy to const memory!");
		myOffset += SerializeAsBinary<T>{}(myState, const_cast<T&>(aInOutData), myBuffer, myOffset); // just cast away constness if state is read
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<T>::operator()(SerializerState aState, T& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (std::is_trivially_copyable_v<T>)
	{
		static constexpr std::size_t numBytes = sizeof(T);

		if (aState == SerializerState::Read)
		{
			assert((aOffset + numBytes) <= aInOutBytes.size() && "Not enough memory to read to!");
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