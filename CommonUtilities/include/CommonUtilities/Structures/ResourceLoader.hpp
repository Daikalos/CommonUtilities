#pragma once

#include <functional>
#include <memory>

namespace CommonUtilities
{
	template<class R>
	class ResourceLoader
	{
	public:
		using Loader = std::function<std::unique_ptr<R>()>;

		ResourceLoader(const Loader& aLoader);
		ResourceLoader(Loader&& aLoader);

		std::unique_ptr<R> operator()() const;
		std::unique_ptr<R> Load() const;

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
	inline std::unique_ptr<R> ResourceLoader<R>::operator()() const
	{
		return myLoader();
	}

	template<class R>
	inline std::unique_ptr<R> ResourceLoader<R>::Load() const
	{
		return myLoader();
	}

	template<class R, typename Func>
	inline ResourceLoader<R> MakeResourceLoader(Func&& aFunc)
	{
		return ResourceLoader<R>(
			[func = std::forward<Func>(aFunc)] -> std::unique_ptr<R>
			{
				auto resource = std::make_unique<R>();

				if (func(*resource))
					return resource;

				return nullptr;
			});
	}
}