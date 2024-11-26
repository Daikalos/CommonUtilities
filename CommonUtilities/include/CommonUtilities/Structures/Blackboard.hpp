#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <cassert>
#include <shared_mutex>

#include <CommonUtilities/Config.h>
#include <CommonUtilities/System/IDGenerator.h>
#include <CommonUtilities/Utility/Concepts.hpp>
#include <CommonUtilities/Structures/FreeVector.hpp>
#include <CommonUtilities/Utility/Clonable.hpp>

namespace CommonUtilities
{
	template<typename IDType = std::string_view, typename Hash = std::hash<IDType>> requires IsHashableType<Hash, IDType>
	class Blackboard
	{
	public:
		Blackboard() = default;
		~Blackboard() = default;

		Blackboard(Blackboard&& aOther);
		Blackboard(const Blackboard& aOther);

		Blackboard& operator=(Blackboard&& aOther);
		Blackboard& operator=(const Blackboard& aOther);

		template<typename T>
		NODISC const T& Get(const IDType& aID) const;

		template<typename T>
		NODISC T& Get(const IDType& aID);

		template<typename T>
		NODISC const T* TryGet(const IDType& aID) const;

		template<typename T>
		NODISC T* TryGet(const IDType& aID);

		template<typename T>
		void Set(const IDType& aID, T&& aValue);

		template<typename T, typename... Args> requires std::constructible_from<T, Args...>
		void Emplace(const IDType& aID, Args&&... someArgs)
		{
			using ValueType = std::decay_t<T>;

			std::scoped_lock lock(myMutex);

			ValueMap<ValueType>& map = FindValueMap<ValueType>();
			map.Emplace(aID, std::forward<Args>(someArgs)...);
		}

		template<typename T>
		void Erase(const IDType& aID);

		template<typename T>
		NODISC bool Has(const IDType& aID) const;

		template<typename T>
		NODISC bool HasType() const;

		void EraseKey(const IDType& aID);

		void Clear();

	private:
		class ValueMapBase
		{
		public:
			ValueMapBase() = default;
			virtual ~ValueMapBase() = default;

			NODISC virtual bool Has(const IDType& aID) = 0;

			virtual void Erase(const IDType& aID) = 0;
			virtual void Clear() = 0;

			NODISC virtual std::unique_ptr<ValueMapBase> Clone() const = 0;
		};

		template<typename T>
		class ValueMap final : public Clonable<ValueMapBase, ValueMap<T>>
		{
		public:
			ValueMap() = default;
			~ValueMap() = default;

			NODISC const T& Get(const IDType& aID) const
			{
				return myValues.at(myIndices.at(Hash{}(aID)));
			}
			NODISC T& Get(const IDType& aID)
			{
				return myValues.at(myIndices.at(Hash{}(aID)));
			}

			NODISC const T* TryGet(const IDType& aID) const
			{
				if (auto it = myIndices.find(Hash{}(aID)); it != myIndices.end())
				{
					return std::addressof(myValues.at(it->second));
				}

				return nullptr;
			}
			NODISC T* TryGet(const IDType& aID)
			{
				return const_cast<T*>(std::as_const(*this).TryGet(aID));
			}

			template<typename... Args>
			void Emplace(const IDType& aID, Args&&... someArgs)
			{
				const std::size_t hash = Hash{}(aID);

				if (const auto it = myIndices.find(hash); it != myIndices.end())
				{
					myValues[it->second] = T{ std::forward<Args>(someArgs)... };
				}
				else
				{
					std::size_t index = myValues.emplace(std::forward<Args>(someArgs)...);
					UNSD auto insert = myIndices.try_emplace(hash, index);
					assert(insert.second);
				}
			}

			void Insert(const IDType& aID, const T& aValue)
			{
				Emplace(aID, aValue);
			}
			void Insert(const IDType& aID, T&& aValue)
			{
				Emplace(aID, std::move(aValue));
			}

			NODISC bool Has(const IDType& aID) override
			{
				return myIndices.find(Hash{}(aID)) != myIndices.end();
			}

			void Erase(const IDType& aID) override
			{
				if (const auto it = myIndices.find(Hash{}(aID)); it != myIndices.end())
				{
					myValues.erase(it->second);
					myIndices.erase(it);
				}
			}
			void Clear() override
			{
				myValues.clear();
				myIndices.clear();
			}

		private:
			using IDIndicesMap = std::unordered_map<std::size_t, std::size_t>;

			FreeVector<T>	myValues;
			IDIndicesMap	myIndices;
		};

		template<typename T>
		NODISC auto FindValueMap() const -> const ValueMap<T>&;

		template<typename T>
		NODISC auto FindValueMap() -> ValueMap<T>&;

		using TypeValueMap = std::unordered_map<std::size_t, std::unique_ptr<ValueMapBase>>;

		TypeValueMap myData;
		mutable std::shared_mutex myMutex;
	};

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline Blackboard<IDType, Hash>::Blackboard(Blackboard&& aOther)
	{
		std::scoped_lock lock(aOther.myMutex);
		myData = std::move(aOther.myData);
	}
	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline Blackboard<IDType, Hash>::Blackboard(const Blackboard& aOther)
	{
		std::shared_lock lock(aOther.myMutex);

		for (const auto& [id, map] : aOther.myData)
		{
			myData[id] = map->Clone();
		}
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline Blackboard<IDType, Hash>& Blackboard<IDType, Hash>::operator=(Blackboard&& aOther)
	{
		if (this == &aOther)
			return *this;

		std::scoped_lock lock1(myMutex);
		std::scoped_lock lock2(aOther.myMutex);

		myData = std::move(aOther.myData);

		return *this;
	}
	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline Blackboard<IDType, Hash>& Blackboard<IDType, Hash>::operator=(const Blackboard& aOther)
	{
		if (this == &aOther)
			return *this;

		std::scoped_lock lock1(myMutex);
		std::shared_lock lock2(aOther.myMutex);

		for (const auto& [id, map] : aOther.myData)
		{
			myData[id] = map->Clone();
		}

		return *this;
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	template<typename T>
	inline const T& Blackboard<IDType, Hash>::Get(const IDType& aID) const
	{
		std::shared_lock lock(myMutex);

		const ValueMap<T>& map = FindValueMap<T>();
		return map.Get(aID);
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	template<typename T>
	inline T& Blackboard<IDType, Hash>::Get(const IDType& aID)
	{
		std::scoped_lock lock(myMutex);

		ValueMap<T>& map = FindValueMap<T>();
		return map.Get(aID);
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	template<typename T>
	inline const T* Blackboard<IDType, Hash>::TryGet(const IDType& aID) const
	{
		std::scoped_lock lock(myMutex);

		const ValueMap<T>& map = FindValueMap<T>();
		return map.TryGet(aID);
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	template<typename T>
	inline T* Blackboard<IDType, Hash>::TryGet(const IDType& aID)
	{
		std::scoped_lock lock(myMutex);

		ValueMap<T>& map = FindValueMap<T>();
		return map.TryGet(aID);
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	template<typename T>
	inline void Blackboard<IDType, Hash>::Set(const IDType& aID, T&& aValue)
	{
		Emplace<T>(aID, std::forward<T>(aValue));
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	template<typename T>
	inline void Blackboard<IDType, Hash>::Erase(const IDType& aID)
	{
		std::scoped_lock lock(myMutex);

		ValueMap<T>& map = FindValueMap<T>();
		map.Erase(aID);
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	template<typename T>
	inline bool Blackboard<IDType, Hash>::Has(const IDType& aID) const
	{
		std::shared_lock lock(myMutex);

		const ValueMap<T>& map = FindValueMap<T>();
		return map.Has(aID);
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	template<typename T>
	inline bool Blackboard<IDType, Hash>::HasType() const
	{
		static constexpr std::size_t key = Type<T>::ID();

		const auto it = myData.find(key);
		return it != myData.end();
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void Blackboard<IDType, Hash>::EraseKey(const IDType& aID)
	{
		std::scoped_lock lock(myMutex);

		for (auto& [id, map] : myData)
		{
			map.Erase(aID);
		}
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	inline void Blackboard<IDType, Hash>::Clear()
	{
		std::scoped_lock lock(myMutex);

		for (auto& [id, map] : myData)
		{
			map.Clear();
		}
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	template<typename T>
	inline auto Blackboard<IDType, Hash>::FindValueMap() const -> const ValueMap<T>&
	{
		static constexpr std::size_t key = Type<T>::ID();
		return *static_cast<ValueMap<T>*>(myData.at(key).get());
	}

	template<typename IDType, typename Hash> requires IsHashableType<Hash, IDType>
	template<typename T>
	inline auto Blackboard<IDType, Hash>::FindValueMap() -> ValueMap<T>&
	{
		static constexpr std::size_t key = Type<T>::ID();

		if (const auto it = myData.find(key); it != myData.end())
		{
			return *static_cast<ValueMap<T>*>(it->second.get());
		}

		UNSD auto insert = myData.try_emplace(key, std::make_unique<ValueMap<T>>());
		assert(insert.second);

		return *static_cast<ValueMap<T>*>(insert.first->second.get());
	}
}