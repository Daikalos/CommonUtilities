#pragma once

#include <memory>
#include <string>

namespace CommonUtilities
{
	inline std::uint32_t JenkinsHash(const std::byte* aKey, std::size_t aLength)
	{
		std::size_t i = 0;
		std::uint32_t hash = 0;

		while (i != aLength) 
		{
			hash += static_cast<std::uint32_t>(aKey[i++]);
			hash += hash << 10;
			hash ^= hash >> 6;
		}

		hash += hash << 3;
		hash ^= hash >> 11;
		hash += hash << 15;

		return hash;
	}

	template<typename T>
	inline std::uint32_t Hash(const T& aKey)
	{
		return JenkinsHash(reinterpret_cast<const std::byte*>(&aKey), sizeof(T));
	}

	template<>
	inline std::uint32_t Hash<std::string>(const std::string& aKey)
	{
		return JenkinsHash(reinterpret_cast<const std::byte*>(aKey.c_str()), aKey.size());
	}

	template<typename Key, typename Value>
	class HashMap
	{
	public:
		HashMap(int aCapacity);

		~HashMap();

		bool Insert(const Key& aKey, const Value& aValue);

		bool Remove(const Key& aKey);

		const Value* Get(const Key& aKey) const;

		Value* Get(const Key& aKey);

	private:
		enum class State
		{
			Empty,
			InUse,
			Removed
		};

		struct Entry
		{
			Entry() : key(), value(), state(State::Empty) {}

			Key		key;
			Value	value;
			State	state;
		};

		std::unique_ptr<Entry[]> myEntries; // please work :)
		int	myCapacity;
	};

	template<typename Key, typename Value>
	inline HashMap<Key, Value>::HashMap(int aCapacity) 
		: myEntries(new Entry[aCapacity > 0 ? aCapacity : 1])
		, myCapacity(aCapacity)
	{

	}

	template<typename Key, typename Value>
	inline HashMap<Key, Value>::~HashMap() = default;

	template<typename Key, typename Value>
	inline bool HashMap<Key, Value>::Insert(const Key& aKey, const Value& aValue)
	{
		std::uint32_t index = Hash(aKey) % myCapacity;

		int iterationCount = 0;
		while (iterationCount++ < myCapacity) // only search for maximum of capacity
		{
			Entry& entry = myEntries[index];

			if (entry.state == State::Empty)
			{
				entry.key	= aKey;
				entry.value = aValue;
				entry.state = State::InUse;
				return true;
			}

			if (!(aKey < entry.key) && !(entry.key < aKey)) // equal
			{
				if (entry.state == State::InUse)
				{
					entry.value = aValue;
					return true;
				}
				
				if (entry.state == State::Removed)
				{
					entry.value = aValue;
					entry.state = State::InUse;
					return true;
				}
			}

			index = (index + 1) % myCapacity; // Go around if it happens
		}

		return false;
	}

	template<typename Key, typename Value>
	inline bool HashMap<Key, Value>::Remove(const Key& aKey)
	{
		std::uint32_t index = Hash(aKey) % myCapacity;

		int iterationCount = 0;
		while (iterationCount++ < myCapacity)
		{
			Entry& entry = myEntries[index];

			if (entry.state == State::Empty)
			{
				return false;
			}

			if (!(aKey < entry.key) && !(entry.key < aKey)) // equal
			{
				if (entry.state == State::InUse)
				{
					entry.state = State::Removed;
					return true;
				}

				if (entry.state == State::Removed)
				{
					return false;
				}
			}

			index = (index + 1) % myCapacity;
		}

		return false;
	}

	template<typename Key, typename Value>
	inline const Value* HashMap<Key, Value>::Get(const Key& aKey) const
	{
		std::uint32_t index = Hash(aKey) % myCapacity;

		int iterationCount = 0;
		while (iterationCount++ < myCapacity)
		{
			const Entry& entry = myEntries[index];

			if (entry.state == State::Empty)
			{
				return nullptr;
			}

			if (!(aKey < entry.key) && !(entry.key < aKey)) // equal
			{
				if (entry.state == State::InUse)
				{
					return &entry.value;
				}

				if (entry.state == State::Removed)
				{
					return nullptr;
				}
			}

			index = (index + 1) % myCapacity;
		}

		return nullptr;
	}

	template<typename Key, typename Value>
	inline Value* HashMap<Key, Value>::Get(const Key& aKey)
	{
		return const_cast<Value*>(static_cast<const HashMap<Key, Value>&>(*this).Get(aKey));
	}
}