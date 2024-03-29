#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <cassert>

#include <CommonUtilities/Config.h>
#include <CommonUtilities/System/IDGenerator.h>
#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Structures/FreeVector.hpp>

namespace CommonUtilities
{
	template<typename IDType = std::string, typename Hash = std::hash<IDType>> requires IsHashable<Hash, IDType>
	class Blackboard
	{
	public:
		Blackboard() = default;
		~Blackboard() = default;

		template<typename T>
		const T& Get(const IDType& aID) const;

		template<typename T>
		T& Get(const IDType& aID);

		template<typename T>
		void Set(const IDType& aID, T&& aValue);

		template<typename T>
		void Erase(const IDType& aID);

		template<typename T>
		bool Has(const IDType& aID) const;

		void EraseKey(const IDType& aID);

		void Clear();

	private:
		class ValueMapBase
		{
		public:
			ValueMapBase() = default;
			virtual ~ValueMapBase() = 0;

			virtual void Erase(const IDType& aID) = 0;
			virtual void Clear() = 0;

		private:

		};

		template<typename T>
		class ValueMap : public ValueMapBase
		{
		public:
			ValueMap() = default;
			~ValueMap() = default;

			const T& Get(const IDType& aID) const;
			T& Get(const IDType& aID);

			void Insert(const IDType& aID, const T& aValue);
			void Insert(const IDType& aID, T&& aValue);

			bool Has(const IDType& aID);

			void Erase(const IDType& aID) override;
			void Clear() override;

		private:
			using IDIndicesMap = std::unordered_map<IDType, std::size_t, Hash>;

			FreeVector<T>	myValues;
			IDIndicesMap	myIndices;

			friend class Blackboard;
		};

		template<typename T>
		auto FindValueMap() -> ValueMap<T>&;

		std::unordered_map<std::size_t, std::unique_ptr<ValueMapBase>> myData;
	};


	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline const T& Blackboard<IDType, Hash>::Get(const IDType& aID) const
	{
		ValueMap<T>& map = FindValueMap<T>();
		return map.Get(aID);
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline T& Blackboard<IDType, Hash>::Get(const IDType& aID)
	{
		ValueMap<T>& map = FindValueMap<T>();
		return map.Get(aID);
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline void Blackboard<IDType, Hash>::Set(const IDType& aID, T&& aValue)
	{
		using Type = std::decay_t<T>;

		ValueMap<Type>& map = FindValueMap<Type>();
		map.Insert(aID, std::forward<T>(aValue));
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline void Blackboard<IDType, Hash>::Erase(const IDType& aID)
	{
		ValueMap<T>& map = FindValueMap<T>();
		map.Erase(aID);
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline bool Blackboard<IDType, Hash>::Has(const IDType& aID) const
	{
		ValueMap<T>& map = FindValueMap<T>();
		return map.Has(aID);
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline void Blackboard<IDType, Hash>::EraseKey(const IDType& aID)
	{
		for (auto& [id, map] : myData)
		{
			map.Erase(aID);
		}
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline void Blackboard<IDType, Hash>::Clear()
	{
		for (auto& [id, map] : myData)
		{
			map.Clear();
		}
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline auto Blackboard<IDType, Hash>::FindValueMap() -> ValueMap<T>&
	{
		static constexpr std::size_t key = id::Type<T>::ID();

		if (const auto it = myData.find(key); it != myData.end())
		{
			return *static_cast<ValueMap<T>*>(it->second.get());
		}

		UNSD auto insert = myData.try_emplace(key, std::make_unique<ValueMap<T>>());
		assert(insert.second);

		return *static_cast<ValueMap<T>*>(insert.first->second.get());
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	inline Blackboard<IDType, Hash>::ValueMapBase::~ValueMapBase() = default;

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline const T& Blackboard<IDType, Hash>::ValueMap<T>::Get(const IDType& aID) const
	{
		return myValues.at(myIndices[aID]);
	}
	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline T& Blackboard<IDType, Hash>::ValueMap<T>::Get(const IDType& aID)
	{
		return myValues.at(myIndices[aID]);
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline void Blackboard<IDType, Hash>::ValueMap<T>::Insert(const IDType& aID, const T& aValue)
	{
		if (const auto it = myIndices.find(aID); it != myIndices.end())
		{
			myValues[it->second] = aValue;
		}
		else
		{
			std::size_t index = myValues.emplace(aValue);
			UNSD auto insert = myIndices.try_emplace(aID, index);
			assert(insert.second);
		}
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline void Blackboard<IDType, Hash>::ValueMap<T>::Insert(const IDType& aID, T&& aValue)
	{
		if (const auto it = myIndices.find(aID); it != myIndices.end())
		{
			myValues[it->second] = std::move(aValue);
		}
		else
		{
			std::size_t index = myValues.emplace(std::move(aValue));
			UNSD auto insert = myIndices.try_emplace(aID, index);
			assert(insert.second);
		}
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline bool Blackboard<IDType, Hash>::ValueMap<T>::Has(const IDType& aID)
	{
		return myIndices.find(aID) != myIndices.end();
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline void Blackboard<IDType, Hash>::ValueMap<T>::Erase(const IDType& aID)
	{
		if (const auto it = myIndices.find(aID); it != myIndices.end())
		{
			myValues.erase(it->second);
			myIndices.erase(it);
		}
	}

	template<typename IDType, typename Hash> requires IsHashable<Hash, IDType>
	template<typename T>
	inline void Blackboard<IDType, Hash>::ValueMap<T>::Clear()
	{
		myValues.clear();
		myIndices.clear();
	}
}