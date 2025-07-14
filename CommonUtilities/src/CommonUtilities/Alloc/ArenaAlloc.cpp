#include <CommonUtilities/Alloc/ArenaAlloc.hpp>

#include <vector>
#include <memory>

using namespace CommonUtilities;

inline constexpr std::size_t DEFAULT_Buffer_CAPACITY = 4096;

class Buffer
{
public:
	Buffer(std::size_t aMinCapacity)
		: myCapacity((std::max)(DEFAULT_Buffer_CAPACITY, aMinCapacity))
		, myData(std::make_unique<std::byte[]>(myCapacity))
	{

	}

	NODISC const std::byte* data() const	{ return myData.get(); }
	NODISC std::byte* data()				{ return const_cast<std::byte*>(std::as_const(*this).data()); }

	NODISC const std::byte* top() const		{ return myData.get() + mySize; }
	NODISC std::byte* top()					{ return const_cast<std::byte*>(std::as_const(*this).top()); }

	NODISC const std::byte* begin() const	{ return myData.get(); }
	NODISC std::byte* begin()				{ return const_cast<std::byte*>(std::as_const(*this).begin()); }

	NODISC const std::byte* end() const		{ return myData.get() + myCapacity; }
	NODISC std::byte* end()					{ return const_cast<std::byte*>(std::as_const(*this).end()); }

	NODISC unsigned ref_count() const		{ return myRefCount; }
	NODISC bool unused() const				{ return ref_count() == 0; }

	void ref()		{ ++myRefCount; }
	void unref()	{ --myRefCount; }

	void resize(std::ptrdiff_t aDiff)	{ mySize += aDiff; }
	void clear()						{ mySize = 0; }

private:
	std::size_t						myCapacity	= 0;
	std::unique_ptr<std::byte[]>	myData		= nullptr;
	std::size_t						mySize		= 0;
	unsigned						myRefCount	= 0;
};

static std::vector<std::unique_ptr<Buffer>> locMemoryBuffers;

using BufferIterator = decltype(locMemoryBuffers)::iterator;

static auto FindBuffer(const std::byte* aHint)
{
	const auto end = locMemoryBuffers.end();
	for (auto it = locMemoryBuffers.begin(); it != end; ++it)
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

static __forceinline bool Fits(const Buffer& aBuffer, std::size_t aNumBytes, std::size_t aAlignment)
{
	aNumBytes += AlignmentOffset(aBuffer.top(), aAlignment);
	return aBuffer.top() + aNumBytes < aBuffer.end();
}

static auto FindBuffer(std::size_t aNumBytes, std::size_t aAlignment, const std::byte* aHint)
{
	const auto end = locMemoryBuffers.end();

	BufferIterator it;

	if (aHint)
	{
		it = FindBuffer(aHint);
		if (it != end && Fits(**it, aNumBytes, aAlignment))
			return it;
	}

	for (it = locMemoryBuffers.begin(); it != end; ++it)
	{
		if (Fits(**it, aNumBytes, aAlignment))
			return it;
	}

	return end;
}

std::byte* details::arena::Allocate(std::size_t aNumBytes, std::size_t aAlignment, const std::byte* aHint)
{
	auto it = FindBuffer(aNumBytes, aAlignment, aHint);
	if (it == locMemoryBuffers.end())
	{
		locMemoryBuffers.emplace_back(std::make_unique<Buffer>(aNumBytes));
		it = std::prev(locMemoryBuffers.end());
	}

	(*it)->resize(AlignmentOffset((*it)->top(), aAlignment));

	std::byte* r = (*it)->top();
	(*it)->resize(aNumBytes);

	(*it)->ref();

	return r;
}

void details::arena::Deallocate(std::byte* aMemory, std::size_t aNumBytes)
{
	auto it = FindBuffer(aMemory);
	if (it == locMemoryBuffers.end())
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