#pragma once

#include "AABB.hpp"
#include "Sphere.hpp"
#include "Capsule.hpp"
#include "Triangle.hpp"
#include "Plane.hpp"
#include "Ray.hpp"

#include <CommonUtilities/System/BinarySerializer.h>

namespace CommonUtilities
{
	template<typename T>
	struct SerializeAsBinary<AABB<T>>
	{
		NODISC std::size_t operator()(SerializerState aState, AABB<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
		NODISC std::size_t operator()(SerializerState aState, const AABB<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename T>
	struct SerializeAsBinary<Sphere<T>>
	{
		NODISC std::size_t operator()(SerializerState aState, Sphere<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
		NODISC std::size_t operator()(SerializerState aState, const Sphere<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename T>
	struct SerializeAsBinary<Capsule<T>>
	{
		NODISC std::size_t operator()(SerializerState aState, Capsule<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
		NODISC std::size_t operator()(SerializerState aState, const Capsule<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename T>
	struct SerializeAsBinary<Triangle<T>>
	{
		NODISC std::size_t operator()(SerializerState aState, Triangle<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
		NODISC std::size_t operator()(SerializerState aState, const Triangle<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename T>
	struct SerializeAsBinary<Plane<T>>
	{
		NODISC std::size_t operator()(SerializerState aState, Plane<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
		NODISC std::size_t operator()(SerializerState aState, const Plane<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename T>
	struct SerializeAsBinary<Ray<T>>
	{
		NODISC std::size_t operator()(SerializerState aState, Ray<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
		NODISC std::size_t operator()(SerializerState aState, const Ray<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset);
	};

	template<typename T>
	inline std::size_t SerializeAsBinary<AABB<T>>::operator()(SerializerState aState, AABB<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			Vector3<T> min, max;

			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, min, aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, max, aInOutBytes, aOffset);

			aInOutData = AABB<T>(aabbMin, aabbMax);
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetMin(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetMax(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}
	template<typename T>
	inline std::size_t SerializeAsBinary<AABB<T>>::operator()(SerializerState aState, const AABB<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			assert(false && "Cannot copy to const memory!");
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetMin(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetMax(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<Sphere<T>>::operator()(SerializerState aState, Sphere<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			Vector3<T> center;
			T radius{0};

			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, center, aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<T>{}(aState, radius, aInOutBytes, aOffset);

			aInOutData = Sphere<T>(center, radius);
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetCenter(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<T>{}(aState, aInOutData.GetRadius(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}
	template<typename T>
	inline std::size_t SerializeAsBinary<Sphere<T>>::operator()(SerializerState aState, const Sphere<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			assert(false && "Cannot copy to const memory!");
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetCenter(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<T>{}(aState, aInOutData.GetRadius(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<Capsule<T>>::operator()(SerializerState aState, Capsule<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			Vector3<T> base, tip;
			T radius{0};

			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, base, aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, tip, aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<T>{}(aState, radius, aInOutBytes, aOffset);

			aInOutData = Capsule<T>(base, tip, radius);
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetBase(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetTip(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<T>{}(aState, aInOutData.GetRadius(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}
	template<typename T>
	inline std::size_t SerializeAsBinary<Capsule<T>>::operator()(SerializerState aState, const Capsule<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			assert(false && "Cannot copy to const memory!");
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetBase(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetTip(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<T>{}(aState, aInOutData.GetRadius(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<Triangle<T>>::operator()(SerializerState aState, Triangle<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			Vector3<T> p0, p1, p2;

			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, p0, aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, p1, aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, p2, aInOutBytes, aOffset);

			aInOutData = Triangle<T>(p0, p1, p2);
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetPoint0(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetPoint1(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetPoint2(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}
	template<typename T>
	inline std::size_t SerializeAsBinary<Triangle<T>>::operator()(SerializerState aState, const Triangle<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			assert(false && "Cannot copy to const memory!");
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetPoint0(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetPoint1(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetPoint2(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<Plane<T>>::operator()(SerializerState aState, Plane<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			Vector3<T> origin;
			Vector3<T> normal;

			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, origin, aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, normal, aInOutBytes, aOffset);

			aInOutData = Plane<T>(origin, normal);
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetOrigin(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetNormal(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}
	template<typename T>
	inline std::size_t SerializeAsBinary<Plane<T>>::operator()(SerializerState aState, const Plane<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			assert(false && "Cannot copy to const memory!");
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetOrigin(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetNormal(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}

	template<typename T>
	inline std::size_t SerializeAsBinary<Ray<T>>::operator()(SerializerState aState, Ray<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			Vector3<T> origin;
			Vector3<T> direction;

			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, origin, aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, direction, aInOutBytes, aOffset);

			aInOutData = Ray<T>(origin, direction);
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetOrigin(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetDirection(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}
	template<typename T>
	inline std::size_t SerializeAsBinary<Ray<T>>::operator()(SerializerState aState, const Ray<T>& aInOutData, std::vector<std::byte>& aInOutBytes, std::size_t aOffset)
	{
		const std::size_t prevOffset = aOffset;

		if (aState == SerializerState::Read)
		{
			assert(false && "Cannot copy to const memory!");
		}
		else
		{
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetOrigin(), aInOutBytes, aOffset);
			aOffset += cu::SerializeAsBinary<Vector3<T>>{}(aState, aInOutData.GetDirection(), aInOutBytes, aOffset);
		}

		const std::size_t numBytes = aOffset - prevOffset;

		return numBytes;
	}
}