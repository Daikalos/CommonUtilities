#pragma once

#include <concepts>
#include <vector>
#include <memory>
#include <ranges>
#include <numeric>
#include <cassert>
#include <tuple>

#include <CommonUtilities/Config.h>
#include <CommonUtilities/System/IDGenerator.h>
#include <CommonUtilities/Utility/ContainerUtils.hpp>

namespace CommonUtilities
{
	// TOOD: add docs

	template<class C>
	class BaseEntity
	{
	public:
		using ComponentPtr		= std::unique_ptr<C>;
		using ComponentIDType	= std::size_t;

		CONSTEXPR BaseEntity() = default;
		CONSTEXPR virtual ~BaseEntity() = 0;

		template<typename T> requires std::derived_from<T, C>
		CONSTEXPR NODISC const T& GetComponent() const;

		template<typename T> requires std::derived_from<T, C>
		CONSTEXPR NODISC T& GetComponent();

		template<typename T> requires std::derived_from<T, C>
		CONSTEXPR NODISC const T* TryGetComponent() const;

		template<typename T> requires std::derived_from<T, C>
		CONSTEXPR NODISC T* TryGetComponent();

		template<typename T> requires std::derived_from<T, C>
		CONSTEXPR NODISC bool IsComponentActive() const;

		template<typename T> requires std::derived_from<T, C>
		CONSTEXPR NODISC bool HasComponent() const;

		template<typename T, typename... Args> requires std::derived_from<T, C> && std::constructible_from<T, Args...>
		CONSTEXPR T* AddComponent(Args&&... someArgs);

		template<typename... Ts> requires (std::derived_from<Ts, C> && ...)
		CONSTEXPR void AddComponents();

		template<typename T, bool MaintainOrder = false> requires std::derived_from<T, C>
		CONSTEXPR bool RemoveComponent();

		template<typename... Ts, bool MaintainOrder = false> requires (std::derived_from<Ts, C> && ...)
		CONSTEXPR void RemoveComponents();

		template<typename T, typename... Args> requires std::derived_from<T, C> && std::constructible_from<T, Args...>
		CONSTEXPR T& SetComponent(Args&&... someArgs);

		template<typename T, typename... Args> requires std::derived_from<T, C> && std::constructible_from<T, Args...>
		CONSTEXPR T* TrySetComponent(Args&&... someArgs);

		template<typename Comp>
		CONSTEXPR void SortComponents(Comp&& aComparison);

		CONSTEXPR void ReserveComponents(std::size_t aCapacity);

		CONSTEXPR void ClearComponents() noexcept;

		CONSTEXPR bool IsComponentsEmpty() const noexcept;
		CONSTEXPR std::size_t ComponentsSize() const noexcept;

		template<typename T> requires std::derived_from<T, C>
		CONSTEXPR void SetComponentActive(bool aFlag);

		template<typename Func> requires HasParametersDecay<Func, C>
		CONSTEXPR void ForEachComponent(Func&& aFunc) const;

		template<typename T> requires std::derived_from<T, C>
		CONSTEXPR NODISC static T& GetStaticComponent();

		template<typename T> requires std::derived_from<T, C>
		CONSTEXPR NODISC static T* TryGetStaticComponent();

		template<typename T> requires std::derived_from<T, C>
		CONSTEXPR NODISC static bool HasStaticComponent();

		template<typename T, typename... Args> requires std::derived_from<T, C> && std::constructible_from<T, Args...>
		CONSTEXPR static T* AddStaticComponent(Args&&... someArgs);

		template<typename T, bool MaintainOrder = false> requires std::derived_from<T, C>
		CONSTEXPR static bool RemoveStaticComponent();

		CONSTEXPR static void ClearStaticComponents() noexcept;

	protected:
		std::vector<ComponentPtr>		myComponents;
		std::vector<ComponentIDType>	myComponentIDs;
		std::vector<bool>				myActiveComponents;

		static std::vector<ComponentPtr> myStaticComponents;
		static std::vector<ComponentIDType> myStaticComponentIDs;
	};

	template<class C>
	CONSTEXPR BaseEntity<C>::~BaseEntity() = default;

	template<class C>
	inline std::vector<typename BaseEntity<C>::ComponentPtr> BaseEntity<C>::myStaticComponents;

	template<class C>
	inline std::vector<typename BaseEntity<C>::ComponentIDType> BaseEntity<C>::myStaticComponentIDs;

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	CONSTEXPR const T& BaseEntity<C>::GetComponent() const
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myComponentIDs, componentID);
		const auto pos = std::distance(it, myComponentIDs.begin());

		return *myComponents[pos];
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	CONSTEXPR T& BaseEntity<C>::GetComponent()
	{
		return const_cast<T&>(std::as_const(*this).template GetComponent<T>()); // const_cast to avoid code duplication
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	CONSTEXPR const T* BaseEntity<C>::TryGetComponent() const
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myComponentIDs, componentID); it != myComponentIDs.end())
		{
			const auto pos = std::distance(it, myComponentIDs.begin());
			return myComponents[pos].get();
		}

		return nullptr;
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	CONSTEXPR T* BaseEntity<C>::TryGetComponent()
	{
		return const_cast<T*>(std::as_const(*this).template TryGetComponent<T>());
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	CONSTEXPR bool BaseEntity<C>::IsComponentActive() const
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myComponentIDs, componentID);
		if (it != myComponentIDs.end())
		{
			const auto pos = std::distance(it, myComponentIDs.begin());
			return myActiveComponents[pos];
		}

		return false;
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	CONSTEXPR bool BaseEntity<C>::HasComponent() const
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myComponentIDs, componentID);
		return it != myComponentIDs.end();
	}

	template<class C>
	template<typename T, typename... Args> requires std::derived_from<T, C>&& std::constructible_from<T, Args...>
	CONSTEXPR T* BaseEntity<C>::AddComponent(Args&&... someArgs)
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myComponentIDs, componentID); it != myComponentIDs.end())
		{
			return nullptr;
		}

		ComponentPtr component = std::make_unique<C>(std::forward<Args>(someArgs)...);

		myComponents.emplace_back(std::move(component));
		myComponentIDs.emplace_back(componentID);
		myActiveComponents.emplace_back(true);

		return myComponents.back().get();
	}

	template<class C>
	template<typename... Ts> requires (std::derived_from<Ts, C> && ...)
	CONSTEXPR void BaseEntity<C>::AddComponents()
	{
		(AddComponent<Ts>(), ...);
	}

	template<class C>
	template<typename T, bool MaintainOrder> requires std::derived_from<T, C>
	CONSTEXPR bool BaseEntity<C>::RemoveComponent()
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myComponentIDs, componentID); it != myComponentIDs.end())
		{
			const auto pos = std::distance(it, myComponentIDs.begin());

			if constexpr (MaintainOrder)
			{
				myComponents.erase(myComponents.begin() + pos);
				myComponentIDs.erase(myComponentIDs.begin() + pos);
				myActiveComponents.erase(myActiveComponents.begin() + pos);
			}
			else
			{
				ctr::EraseCyclicAt(myComponents, pos);
				ctr::EraseCyclicAt(myComponentIDs, pos);
				ctr::EraseCyclicAt(myActiveComponents, pos);
			}

			return true;
		}

		return false;
	}

	template<class C>
	template<typename... Ts, bool MaintainOrder> requires (std::derived_from<Ts, C> && ...)
	CONSTEXPR void BaseEntity<C>::RemoveComponents()
	{
		(RemoveComponent<Ts, MaintainOrder>, ...);
	}

	template<class C>
	template<typename T, typename... Args> requires std::derived_from<T, C> && std::constructible_from<T, Args...>
	CONSTEXPR T& BaseEntity<C>::SetComponent(Args&&... someArgs)
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myComponentIDs, componentID);
		const auto pos = std::distance(it, myComponentIDs.begin());

		return (*myComponents[pos] = T{ std::forward<Args>(someArgs)... });
	}

	template<class C>
	template<typename T, typename... Args> requires std::derived_from<T, C>&& std::constructible_from<T, Args...>
	CONSTEXPR T* BaseEntity<C>::TrySetComponent(Args&&... someArgs)
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myComponentIDs, componentID);
		if (it != myComponentIDs.end())
		{
			const auto pos = std::distance(it, myComponentIDs.begin());

			T* compPtr = myComponents[pos].get();
			*compPtr = T{ std::forward<Args>(someArgs)... };

			return compPtr;
		}

		return nullptr;
	}

	template<class C>
	template<typename Comp>
	CONSTEXPR void BaseEntity<C>::SortComponents(Comp&& aCompare)
	{
		std::vector<std::size_t> indices(myComponents.size());
		std::iota(indices.begin(), indices.end(), 0);

		std::ranges::sort(indices,
			[this, comp = std::forward<Comp>(aCompare)](std::size_t aFirst, std::size_t aSecond)
			{
				return comp(*myComponents[aFirst], *myComponents[aSecond]);
			});

		ctr::ApplyPermutation(myComponents, indices);
		ctr::ApplyPermutation(myComponentIDs, indices);
		ctr::ApplyPermutation(myActiveComponents, indices);
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	CONSTEXPR void BaseEntity<C>::SetComponentActive(bool aFlag)
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myComponentIDs, componentID); it != myComponentIDs.end())
		{
			const auto pos = std::distance(it, myComponentIDs.begin());
			myActiveComponents[pos] = aFlag;
		}
	}

	template<class C>
	CONSTEXPR void BaseEntity<C>::ReserveComponents(std::size_t aCapacity)
	{
		myComponents.reserve(aCapacity);
		myComponentIDs.reserve(aCapacity);
		myActiveComponents.reserve(aCapacity);
	}

	template<class C>
	CONSTEXPR void BaseEntity<C>::ClearComponents() noexcept
	{
		myComponents.clear();
		myComponentIDs.clear();
		myActiveComponents.clear();
	}

	template<class C>
	CONSTEXPR bool BaseEntity<C>::IsComponentsEmpty() const noexcept
	{
		return myComponents.empty();
	}

	template<class C>
	CONSTEXPR std::size_t BaseEntity<C>::ComponentsSize() const noexcept
	{
		return myComponents.size();
	}

	template<class C>
	template<typename Func> requires HasParametersDecay<Func, C>
	CONSTEXPR void BaseEntity<C>::ForEachComponent(Func&& aFunc) const
	{
		for (std::size_t index = 0; index < myComponents.size(); ++index)
		{
			if (!myActiveComponents[index])
				continue;

			aFunc(*myComponents[index]);
		}
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	CONSTEXPR T& BaseEntity<C>::GetStaticComponent()
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myStaticComponentIDs, componentID);
		const auto pos = std::distance(it, myStaticComponentIDs.begin());

		return *myStaticComponents[pos];
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	CONSTEXPR T* BaseEntity<C>::TryGetStaticComponent()
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myStaticComponentIDs, componentID); it != myStaticComponentIDs.end())
		{
			const auto pos = std::distance(it, myStaticComponentIDs.begin());
			return myStaticComponents[pos].get();
		}

		return nullptr;
	}

	template<class C>
	template<typename T> requires std::derived_from<T, C>
	CONSTEXPR bool BaseEntity<C>::HasStaticComponent()
	{
		static constexpr auto componentID = id::Type<T>::ID();

		const auto it = std::ranges::find(myStaticComponentIDs, componentID);
		return it != myStaticComponentIDs.end();
	}
	template<class C>
	template<typename T, typename... Args> requires std::derived_from<T, C>&& std::constructible_from<T, Args...>
	CONSTEXPR T* BaseEntity<C>::AddStaticComponent(Args&&... someArgs)
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myStaticComponentIDs, componentID); it != myStaticComponentIDs.end())
		{
			return nullptr;
		}

		ComponentPtr component = std::make_unique<C>(std::forward<Args>(someArgs)...);

		myStaticComponents.emplace_back(std::move(component));
		myStaticComponentIDs.emplace_back(componentID);

		return myStaticComponents.back().get();
	}
	template<class C>
	template<typename T, bool MaintainOrder> requires std::derived_from<T, C>
	CONSTEXPR bool BaseEntity<C>::RemoveStaticComponent()
	{
		static constexpr auto componentID = id::Type<T>::ID();

		if (const auto it = std::ranges::find(myStaticComponentIDs, componentID); it == myStaticComponentIDs.end())
		{
			const auto pos = std::distance(it, myStaticComponentIDs.begin());

			if constexpr (MaintainOrder)
			{
				myStaticComponents.erase(myStaticComponents.begin() + pos);
				myStaticComponentIDs.erase(myStaticComponentIDs.begin() + pos);
			}
			else
			{
				ctr::EraseCyclicAt(myStaticComponents, pos);
				ctr::EraseCyclicAt(myStaticComponentIDs, pos);
			}

			return true;
		}

		return false;
	}

	template<class C>
	CONSTEXPR void BaseEntity<C>::ClearStaticComponents() noexcept
	{
		myStaticComponents.clear();
		myStaticComponentIDs.clear();
	}
}
