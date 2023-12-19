#pragma once

#include <future>
#include <memory>
#include <unordered_map>
#include <thread>
#include <shared_mutex>

#include <CommonUtilities/Utility/NonCopyable.h>
#include <CommonUtilities/Config.h>

#include "ResourceLoader.hpp"

namespace CommonUtilities
{
	namespace res
	{
		enum class LoadStrategy
		{
			New,
			Reuse,
			Reload
		};
	}

	/// Holds resources, for example, fonts, textures, and sounds.
	///
	template <class R, typename I>
	class ResourceHolder : private NonCopyable
	{
	public:
		using ResourcePtr = typename std::unique_ptr<R>;

		using ReturnType = R&;
		using ConstReturnType = const R&;

		ResourceHolder();
		~ResourceHolder();

		auto operator[](const I& aID) -> ReturnType;
		auto operator[](const I& aID) const -> ConstReturnType;

		auto Get(const I& aID) -> ReturnType;
		auto Get(const I& aID) const -> ConstReturnType;

		auto Acquire(const I& aID, const ResourceLoader<R>& aLoader, res::LoadStrategy aStrat = res::LoadStrategy::New) -> ReturnType;
		auto AcquireAsync(const I& aID, const ResourceLoader<R>& aLoader, res::LoadStrategy aStrat = res::LoadStrategy::New) -> std::future<ReturnType>;

		void Release(const I& aID);

		bool Contains(const I& aID) const;

	private:
		void ReleaseImpl(const I& aID);
		auto Load(const I& aID, const ResourceLoader<R>& aLoader) -> ReturnType;
		auto Insert(const I& aID, ResourcePtr&& aResource) -> ReturnType;

		std::unordered_map<I, ResourcePtr> myResources;
		mutable std::shared_mutex myMutex;
	};

	template<class R, typename I>
	inline ResourceHolder<R, I>::ResourceHolder() = default;

	template<class R, typename I>
	inline ResourceHolder<R, I>::~ResourceHolder() = default;

	template<class R, typename I>
	inline auto ResourceHolder<R, I>::operator[](const I& aID) -> ReturnType
	{
		return Get(aID);
	}

	template<class R, typename I>
	inline auto ResourceHolder<R, I>::operator[](const I& aID) const -> ConstReturnType
	{
		return Get(aID);
	}

	template<class R, typename I>
	inline auto ResourceHolder<R, I>::Get(const I& aID) -> ReturnType
	{
		return const_cast<ReturnType>(std::as_const(*this).Get(aID));
	}
	template<class R, typename I>
	inline auto ResourceHolder<R, I>::Get(const I& aID) const -> ConstReturnType
	{
		std::shared_lock lock(myMutex);

		const auto it = myResources.find(aID);
		if (it == myResources.end())
			throw std::runtime_error("Resource does not exist");

		return *it->second;
	}

	template<class R, typename I>
	inline auto ResourceHolder<R, I>::Acquire(const I& aID, const ResourceLoader<R>& aLoader, res::LoadStrategy aStrat) -> ReturnType
	{
		std::scoped_lock lock(myMutex);

		const auto it = myResources.find(aID);
		if (it == myResources.end())
			return Load(aID, aLoader);

		switch (aStrat)
		{
			case res::LoadStrategy::New:
				throw std::runtime_error("Failed to load, already exists in container");
			case res::LoadStrategy::Reload:
				ReleaseImpl(aID);
				return Load(aID, aLoader);
			default: // reuse as default
				return *it->second;
		}
	}

	template<class R, typename I>
	inline auto ResourceHolder<R, I>::AcquireAsync(const I& aID, const ResourceLoader<R>& aLoader, res::LoadStrategy aStrat) -> std::future<ReturnType>
	{
		return std::async(std::launch::async,
			[this](I aID, ResourceLoader<R> aLoader, res::LoadStrategy aStrat) -> ReturnType
			{
				ResourcePtr resource = aLoader(); // we load it first for async benefits

				if (!resource)
					throw std::runtime_error("Failed to load resource");

				std::scoped_lock lock(myMutex); // guard race condition for resources

				const auto it = myResources.find(aID);
				if (it == myResources.end())
					return Insert(aID, std::move(resource));

				switch (aStrat)
				{
					case res::LoadStrategy::New:
						throw std::runtime_error("Failed to load, already exists in container");
					case res::LoadStrategy::Reload:
						ReleaseImpl(aID);
						return Insert(aID, std::move(resource));
					default: // reuse as default
						return *it->second;
				}
			}, aID, aLoader, aStrat); // copy id and loader just to be safe
	}

	template<class R, typename I>
	inline void ResourceHolder<R, I>::Release(const I& aID)
	{
		std::scoped_lock lock(myMutex);
		ReleaseImpl(aID);
	}

	template<class R, typename I>
	inline bool ResourceHolder<R, I>::Contains(const I& aID) const
	{
		std::shared_lock lock(myMutex);
		return myResources.contains(aID);
	}

	template<class R, typename I>
	inline void ResourceHolder<R, I>::ReleaseImpl(const I& aID)
	{
		const auto it = myResources.find(aID);
		if (it == myResources.end())
			throw std::runtime_error("Resource does not exist");

		myResources.erase(it);
	}

	template<class R, typename I>
	inline auto ResourceHolder<R, I>::Load(const I& aID, const ResourceLoader<R>& aLoader) -> ReturnType
	{
		ResourcePtr resource = aLoader();

		if (!resource)
			throw std::runtime_error("Failed to load resource");

		return Insert(aID, std::move(resource));
	}

	template<class R, typename I>
	inline auto ResourceHolder<R, I>::Insert(const I& aID, ResourcePtr&& aResource) -> ReturnType
	{
		return *myResources.try_emplace(aID, std::move(aResource)).first->second;
	}
}
