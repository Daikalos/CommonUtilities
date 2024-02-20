#pragma once

#include <functional>

#include <CommonUtilities/Config.h>

namespace CommonUtilities
{
	template<class R>
	class ResourceLoader
	{
	public:
		using Loader = std::function<R()>;

		ResourceLoader(const Loader& aLoader);
		ResourceLoader(Loader&& aLoader);

		NODISC R operator()() const;
		NODISC R Load() const;

	private:
		Loader myLoader;
	};

	template<class R>
	inline ResourceLoader<R>::ResourceLoader(const Loader& aLoader)
		: myLoader(aLoader) { }

	template<class R>
	inline ResourceLoader<R>::ResourceLoader(Loader&& aLoader)
		: myLoader(std::move(aLoader)) { }

	template<class R>
	inline R ResourceLoader<R>::operator()() const
	{
		return myLoader();
	}

	template<class R>
	inline R ResourceLoader<R>::Load() const
	{
		return myLoader();
	}

	template<class R, typename Func>
	inline ResourceLoader<R> MakeResourceLoader(Func&& aFunc)
	{
		return ResourceLoader<R>(
			[func = std::forward<Func>(aFunc)]() -> R
			{
				R resource{};

				if (func(resource))
					return resource;

				return nullptr;
			});
	}
}