#pragma once

#include <vector>
#include <string>
#include <memory>
#include <span>
#include <cassert>

#include <CommonUtilities/Utility/Traits.h>
#include <CommonUtilities/Config.h>

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
	struct SerializeAsBinary<std::vector<T>>
	{
		NODISC std::size_t operator()(SerializerState aState, std::vector<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (std::is_trivially_copyable_v<T>);

		NODISC std::size_t operator()(SerializerState aState, const std::vector<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (std::is_trivially_copyable_v<T>);

		NODISC std::size_t operator()(SerializerState aState, std::vector<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (!std::is_trivially_copyable_v<T>); // user must provide their own custom specialization for this type to work

		NODISC std::size_t operator()(SerializerState aState, const std::vector<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
			requires (!std::is_trivially_copyable_v<T>);
	};

	template<typename T, typename U> requires (!std::is_trivially_copyable_v<T> || !std::is_trivially_copyable_v<U>)
	struct SerializeAsBinary<std::pair<T, U>>
	{
		NODISC std::size_t operator()(SerializerState aState, std::pair<T, U>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
		NODISC std::size_t operator()(SerializerState aState, const std::pair<T, U>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename... Ts> requires (!std::is_trivially_copyable_v<Ts> || ...)
	struct SerializeAsBinary<std::tuple<Ts...>>
	{
		NODISC std::size_t operator()(SerializerState aState, std::tuple<Ts...>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
		NODISC std::size_t operator()(SerializerState aState, const std::tuple<Ts...>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);

	private:
		template<std::size_t I = 0>
		std::size_t SerializeTuple(SerializerState aState, std::tuple<Ts...>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	class BinarySerializer
	{
	public:
		BinarySerializer() = delete;
		virtual ~BinarySerializer() = default;

		NODISC SerializerState GetState() const noexcept { return myState; }
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

		COMMON_UTILITIES_API void Clear();
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
	inline std::size_t SerializeAsBinary<std::vector<T>>::operator()(SerializerState aState, std::vector<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (std::is_trivially_copyable_v<T>)
	{
		static constexpr std::size_t typeSize = sizeof(T);

		std::size_t numBytes = 0;

		if (aState == SerializerState::Read)
		{
			std::size_t numElements = 0;
			memcpy_s(&numElements, sizeof(std::size_t), aInOutBytes.data() + aOffset, sizeof(std::size_t));

			numBytes = typeSize * numElements;

			assert((aOffset + numBytes + sizeof(std::size_t)) <= aInOutBytes.size() && "Not enough memory to read from!");
			
			if (aInOutData.size() < numElements)
			{
				aInOutData.resize(numElements);
			}

			aOffset += sizeof(std::size_t);

			memcpy_s(aInOutData.data(), numBytes, aInOutBytes.data() + aOffset, numBytes);
		}
		else
		{
			std::size_t numElements = aInOutData.size();

			numBytes = typeSize * numElements;

			aInOutBytes.resize(aOffset + numBytes + sizeof(std::size_t));

			memcpy_s(aInOutBytes.data() + aOffset, sizeof(std::size_t), &numElements, sizeof(std::size_t));

			aOffset += sizeof(std::size_t);

			memcpy_s(aInOutBytes.data() + aOffset, numBytes, aInOutData.data(), numBytes);
		}

		return numBytes + sizeof(std::size_t);
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<std::vector<T>>::operator()(SerializerState aState, const std::vector<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (std::is_trivially_copyable_v<T>)
	{
		static constexpr std::size_t typeSize = sizeof(T);

		std::size_t numBytes = 0;

		if (aState == SerializerState::Read)
		{
			assert(false && "Cannot copy to const memory!");
		}
		else
		{
			std::size_t numElements = aInOutData.size();

			numBytes = typeSize * numElements;

			aInOutBytes.resize(aOffset + numBytes + sizeof(std::size_t));

			memcpy_s(aInOutBytes.data() + aOffset, sizeof(std::size_t), &numElements, sizeof(std::size_t));

			aOffset += sizeof(std::size_t);

			memcpy_s(aInOutBytes.data() + aOffset, numBytes, aInOutData.data(), numBytes);
		}

		return numBytes + sizeof(std::size_t);
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<std::vector<T>>::operator()(SerializerState aState, std::vector<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (!std::is_trivially_copyable_v<T>)
	{
		std::size_t numBytes = 0;

		if (aState == SerializerState::Read)
		{
			std::size_t numElements = 0;
			memcpy_s(&numElements, sizeof(std::size_t), aInOutBytes.data() + aOffset, sizeof(std::size_t));

			if (aInOutData.size() < numElements)
			{
				aInOutData.resize(numElements);
			}

			numBytes += sizeof(std::size_t);

			for (std::size_t i = 0; i < numElements; ++i)
			{
				numBytes += SerializeAsBinary<T>{}(aState, aInOutData[i], aInOutBytes, aOffset + numBytes);
			}
		}
		else
		{
			aInOutBytes.resize(aOffset + sizeof(std::size_t));

			std::size_t numElements = aInOutData.size();
			memcpy_s(aInOutBytes.data() + aOffset, sizeof(std::size_t), &numElements, sizeof(std::size_t));

			numBytes += sizeof(std::size_t);

			for (std::size_t i = 0; i < numElements; ++i)
			{
				numBytes += SerializeAsBinary<T>{}(aState, aInOutData[i], aInOutBytes, aOffset + numBytes);
			}
		}

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<std::vector<T>>::operator()(SerializerState aState, const std::vector<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
		requires (!std::is_trivially_copyable_v<T>)
	{
		std::size_t numBytes = 0;

		if (aState == SerializerState::Read)
		{
			assert(false && "Cannot copy to const memory!");
		}
		else
		{
			aInOutBytes.resize(aOffset + sizeof(std::size_t));

			std::size_t numElements = aInOutData.size();
			memcpy_s(aInOutBytes.data() + aOffset, sizeof(std::size_t), &numElements, sizeof(std::size_t));

			numBytes += sizeof(std::size_t);

			for (std::size_t i = 0; i < numElements; ++i)
			{
				numBytes += SerializeAsBinary<T>{}(aState, aInOutData[i], aInOutBytes, aOffset + numBytes);
			}
		}

		return numBytes;
	}

	template<typename T, typename U> requires (!std::is_trivially_copyable_v<T> || !std::is_trivially_copyable_v<U>)
	inline std::size_t SerializeAsBinary<std::pair<T, U>>::operator()(SerializerState aState, std::pair<T, U>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		aOffset += cu::SerializeAsBinary<T>{}(aState, aInOutData.first, aInOutBytes, aOffset);
		aOffset += cu::SerializeAsBinary<U>{}(aState, aInOutData.second, aInOutBytes, aOffset);

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}

	template<typename T, typename U> requires (!std::is_trivially_copyable_v<T> || !std::is_trivially_copyable_v<U>)
	inline std::size_t SerializeAsBinary<std::pair<T, U>>::operator()(SerializerState aState, const std::pair<T, U>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		aOffset += cu::SerializeAsBinary<T>{}(aState, aInOutData.first, aInOutBytes, aOffset);
		aOffset += cu::SerializeAsBinary<U>{}(aState, aInOutData.second, aInOutBytes, aOffset);

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}

	template<typename... Ts> requires (!std::is_trivially_copyable_v<Ts> || ...)
	inline std::size_t SerializeAsBinary<std::tuple<Ts...>>::operator()(SerializerState aState, std::tuple<Ts...>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		aOffset = SerializeTuple<>(aState, aInOutData, aInOutBytes, aOffset);

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}

	template<typename... Ts> requires (!std::is_trivially_copyable_v<Ts> || ...)
	inline std::size_t SerializeAsBinary<std::tuple<Ts...>>::operator()(SerializerState aState, const std::tuple<Ts...>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		aOffset = SerializeTuple<>(aState, aInOutData, aInOutBytes, aOffset);

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}

	template<typename... Ts> requires (!std::is_trivially_copyable_v<Ts> || ...)
	template<std::size_t I>
	inline std::size_t SerializeAsBinary<std::tuple<Ts...>>::SerializeTuple(SerializerState aState, std::tuple<Ts...>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		if constexpr (I != sizeof...(Ts))
		{
			aOffset += cu::SerializeAsBinary<std::tuple_element_t<I, std::tuple<Ts...>>>{}(aState, std::get<I>(aInOutData), aInOutBytes, aOffset);
			return SerializeTuple<I + 1>(aState, aInOutData, aInOutBytes, aOffset);
		}
		else
		{
			return aOffset;
		}
	}

	template<typename T>
	inline BinaryWriteSerializer& operator<<(BinaryWriteSerializer& aBinaryWriteSerializer, const T& aInData)
	{
		aBinaryWriteSerializer.Serialize(aInData);
		return aBinaryWriteSerializer;
	}

	template<typename T>
	inline BinaryReadSerializer& operator>>(BinaryReadSerializer& aBinaryReadSerializer, T& aOutData)
	{
		aBinaryReadSerializer.Serialize(aOutData);
		return aBinaryReadSerializer;
	}
}