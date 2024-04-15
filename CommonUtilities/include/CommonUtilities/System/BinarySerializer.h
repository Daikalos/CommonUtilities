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
		Read,	// Copies bytes from buffer onto type
		Write	// Copes bytes from type onto buffer
	};

	template<typename T>
	struct SerializeAsBinary
	{
		NODISC std::size_t operator()(SerializerState aState, T& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (std::is_trivially_copyable_v<T>); // trivially copyable is required to prevent UB

		NODISC std::size_t operator()(SerializerState aState, const T& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (std::is_trivially_copyable_v<T>); 
	};

	template<>
	struct SerializeAsBinary<std::string>
	{
		NODISC std::size_t operator()(SerializerState aState, std::string& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
		NODISC std::size_t operator()(SerializerState aState, const std::string& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename T>
	struct SerializeAsBinary<std::span<T>>
	{
		NODISC std::size_t operator()(SerializerState aState, std::span<T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (std::is_trivially_copyable_v<T>);

		NODISC std::size_t operator()(SerializerState aState, std::span<const T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (std::is_trivially_copyable_v<T>);

		NODISC std::size_t operator()(SerializerState aState, std::span<T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (!std::is_trivially_copyable_v<T>); // user must provide their own custom specialization for this type to work

		NODISC std::size_t operator()(SerializerState aState, std::span<const T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (!std::is_trivially_copyable_v<T>);
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
		((myOffset += SerializeAsBinary<std::decay_t<Ts>>{}(myState, aInOutData, myBuffer, myOffset)), ...);
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<T>::operator()(SerializerState aState, T& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (std::is_trivially_copyable_v<T>)
	{
		static constexpr std::size_t numBytes = sizeof(T);

		if (aState == SerializerState::Read)
		{
			assert((aOffset + numBytes) <= aInOutBytes.size() && "Not enough memory to read from!");
			memcpy_s(&aInOutData, numBytes, aInOutBytes.data() + aOffset, numBytes);
		}
		else
		{
			aInOutBytes.resize(aOffset + numBytes);
			memcpy_s(aInOutBytes.data() + aOffset, numBytes, &aInOutData, numBytes);
		}

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<T>::operator()(SerializerState aState, const T& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (std::is_trivially_copyable_v<T>)
	{
		static constexpr std::size_t numBytes = sizeof(T);

		if (aState == SerializerState::Read)
		{
			assert(false && "Cannot copy to const memory!");
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
		requires (std::is_trivially_copyable_v<T>)
	{
		static constexpr std::size_t typeSize = sizeof(T);

		const std::size_t numBytes = typeSize * aInOutData.size(); // user has to make sure that size matches the buffer correctly

		if (aState == SerializerState::Read)
		{
			assert((aOffset + numBytes) <= aInOutBytes.size() && "Not enough memory to read from!");
			memcpy_s(aInOutData.data(), numBytes, aInOutBytes.data() + aOffset, numBytes);
		}
		else
		{
			aInOutBytes.resize(aOffset + numBytes);
			memcpy_s(aInOutBytes.data() + aOffset, numBytes, aInOutData.data(), numBytes);
		}

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<std::span<T>>::operator()(SerializerState aState, std::span<const T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (std::is_trivially_copyable_v<T>)
	{
		static constexpr std::size_t typeSize = sizeof(T);

		const std::size_t numBytes = typeSize * aInOutData.size();

		if (aState == SerializerState::Read)
		{
			assert(false && "Cannot copy to const memory!");
		}
		else
		{
			aInOutBytes.resize(aOffset + numBytes);
			memcpy_s(aInOutBytes.data() + aOffset, numBytes, aInOutData.data(), numBytes);
		}

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<std::span<T>>::operator()(SerializerState aState, std::span<T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (!std::is_trivially_copyable_v<T>)
	{
		std::size_t numBytes = 0;
		for (T& item : aInOutData)
		{
			numBytes += SerializeAsBinary<T>{}(aState, item, aInOutBytes, aOffset + numBytes);
		}

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<std::span<T>>::operator()(SerializerState aState, std::span<const T> aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (!std::is_trivially_copyable_v<T>)
	{
		std::size_t numBytes = 0;
		for (const T& item : aInOutData)
		{
			numBytes += SerializeAsBinary<T>{}(aState, item, aInOutBytes, aOffset + numBytes);
		}

		return numBytes;
	}
}