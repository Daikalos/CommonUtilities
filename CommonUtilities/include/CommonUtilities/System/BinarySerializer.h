#pragma once

#include <vector>
#include <string>
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

	template<>
	struct SerializeAsBinary<const std::string>
	{
		NODISC std::size_t operator()(SerializerState aState, const std::string& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename T>
	struct SerializeAsBinary<std::span<T>>
	{
		NODISC std::size_t operator()(SerializerState aState, std::span<T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename T>
	struct SerializeAsBinary<std::span<const T>>
	{
		NODISC std::size_t operator()(SerializerState aState, std::span<const T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	class BinarySerializer
	{
	public:
		BinarySerializer() = delete;
		virtual ~BinarySerializer() = default;

		NODISC std::size_t GetOffset() const noexcept { return myOffset; }

		template<typename... Ts>
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

		COMMON_UTILITIES_API void ReserveBytesToFit(std::size_t aNumBytesToFit);
		COMMON_UTILITIES_API void FitBufferToOffset();
	};

	template<typename... Ts>
	inline void BinarySerializer::Serialize(Ts&... aInOutData)
	{
		((myOffset += SerializeAsBinary<Ts>{}(myState, aInOutData, myBuffer, myOffset)), ...);
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<T>::operator()(SerializerState aState, T& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (std::is_trivially_copyable_v<T>)
	{
		static constexpr std::size_t numBytes = sizeof(T);

		if (aState == SerializerState::Read)
		{
			if constexpr (!std::is_const_v<T>)
			{
				assert((aOffset + numBytes) <= aInOutBytes.size() && "Not enough memory to read to!");
				memcpy_s(&aInOutData, numBytes, aInOutBytes.data() + aOffset, numBytes);
			}
			else
			{
				assert(false && "Cannot copy to const memory!");
			}
		}
		else
		{
			aInOutBytes.resize(aOffset + numBytes);
			memcpy_s(aInOutBytes.data() + aOffset, numBytes, &aInOutData, numBytes);
		}

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<std::span<T>>::operator()(SerializerState aState, std::span<T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		std::size_t numBytes = 0;
		for (T& item : aInOutData)
		{
			numBytes += SerializeAsBinary<T>{}(aState, item, aInOutBytes, aOffset + numBytes);
		}

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<std::span<const T>>::operator()(SerializerState aState, std::span<const T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		std::size_t numBytes = 0;
		for (const T& item : aInOutData)
		{
			numBytes += SerializeAsBinary<T>{}(aState, item, aInOutBytes, aOffset + numBytes);
		}

		return numBytes;
	}
}