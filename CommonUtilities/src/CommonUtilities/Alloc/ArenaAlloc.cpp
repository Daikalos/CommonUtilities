#include <CommonUtilities/Alloc/ArenaAlloc.hpp>

#include <vector>
#include <memory>

#include <CommonUtilities/System/WindowsHeader.h>

using namespace CommonUtilities;

inline constexpr std::size_t DEFAULT_REGION_CAPACITY = 4096;

static std::byte* AllocateMemory(std::size_t aBytes)
{
	return new std::byte[aBytes];
}

static void DeallocateMemory(std::byte* aMemory)
{
	delete[] aMemory;
}

class MemoryRegion
{
public:
	MemoryRegion(std::size_t aMinCapacity)
		: myCapacity((std::max)(DEFAULT_REGION_CAPACITY, aMinCapacity))
		, myData(AllocateMemory(myCapacity))
	{

	}

	~MemoryRegion()
	{
		DeallocateMemory(myData);
	}

	NODISC const std::byte* data() const	{ return myData; }
	NODISC std::byte* data()				{ return const_cast<std::byte*>(std::as_const(*this).data()); }

	NODISC const std::byte* top() const		{ return myData + mySize; }
	NODISC std::byte* top()					{ return const_cast<std::byte*>(std::as_const(*this).top()); }

	NODISC const std::byte* begin() const	{ return myData; }
	NODISC std::byte* begin()				{ return const_cast<std::byte*>(std::as_const(*this).begin()); }

	NODISC const std::byte* end() const		{ return myData + myCapacity; }
	NODISC std::byte* end()					{ return const_cast<std::byte*>(std::as_const(*this).end()); }

	NODISC unsigned ref_count() const		{ return myRefCount; }
	NODISC bool unused() const				{ return ref_count() == 0; }

	void ref()		{ ++myRefCount; }
	void unref()	{ --myRefCount; }

	void resize(std::ptrdiff_t aDiff)	{ mySize += aDiff; }
	void clear()						{ mySize = 0; }

private:
	std::size_t	myCapacity	= 0;
	std::byte*	myData		= nullptr;
	std::size_t	mySize		= 0;
	unsigned	myRefCount	= 0;
};

static std::vector<std::unique_ptr<MemoryRegion>> locMemoryRegions;

using RegionIterator = decltype(locMemoryRegions)::iterator;

static auto FindRegion(const std::byte* aHint)
{
	const auto end = locMemoryRegions.end();
	for (auto it = locMemoryRegions.begin(); it != end; ++it)
	{
		if (aHint >= (*it)->data() && aHint < (*it)->top())
			return it;
	}

	return end;
}

static __forceinline std::ptrdiff_t AlignmentOffset(const std::byte* aMemory, std::size_t aAlignment)
{
	const auto off = aAlignment - reinterpret_cast<std::uintptr_t>(aMemory) % aAlignment;
	return off == aAlignment ? 0 : off;
}

static __forceinline bool Fits(const MemoryRegion& aRegion, std::size_t aNumBytes, std::size_t aAlignment)
{
	aNumBytes += AlignmentOffset(aRegion.top(), aAlignment);
	return aRegion.top() + aNumBytes < aRegion.end();
}

static auto FindRegion(std::size_t aNumBytes, std::size_t aAlignment, const std::byte* aHint)
{
	const auto end = locMemoryRegions.end();

	RegionIterator it;

	if (aHint)
	{
		it = FindRegion(aHint);
		if (it != end && Fits(**it, aNumBytes, aAlignment))
			return it;
	}

	for (it = locMemoryRegions.begin(); it != end; ++it)
	{
		if (Fits(**it, aNumBytes, aAlignment))
			return it;
	}

	return end;
}

std::byte* details::arena::Allocate(std::size_t aNumBytes, std::size_t aAlignment, const std::byte* aHint)
{
	auto it = FindRegion(aNumBytes, aAlignment, aHint);
	if (it == locMemoryRegions.end())
	{
		locMemoryRegions.emplace_back(std::make_unique<MemoryRegion>(aNumBytes));
		it = std::prev(locMemoryRegions.end());
	}

	(*it)->resize(AlignmentOffset((*it)->top(), aAlignment));

	std::byte* r = (*it)->top();
	(*it)->resize(aNumBytes);

	(*it)->ref();

	return r;
}

void details::arena::Deallocate(std::byte* aMemory, std::size_t aNumBytes)
{
	auto it = FindRegion(aMemory);
	if (it == locMemoryRegions.end())
		return;

	(*it)->unref();

	if ((*it)->unused())
	{
		(*it)->clear();
	}
	else if (aMemory + aNumBytes == (*it)->top())
	{
		(*it)->resize(-(std::ptrdiff_t)aNumBytes);
	}
}