#pragma once

#include <cfloat>
#include <cmath>
#include <array>

#include "Plane.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "AABB3D.hpp"
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	inline bool IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay, Vector3<T>& outIntersectionPoint)
	{
		T numen = Vector3<T>::Direction(aRay.GetOrigin(), aPlane.GetOrigin()).Dot(aPlane.GetNormal());
		T denom = aPlane.GetNormal().Dot(aRay.GetDirection());

		if (static_cast<T>(std::abs(denom)) <= static_cast<T>(FLT_EPSILON))
		{
			// if the numenator is zero then the origin of the ray lies on plane, which it must if parallel to the plane and allow intersection
			return numen == 0;
		}

		T t = numen / denom;

		if (t < 0)
		{
			// ray points away from plane
			return false;
		}

		outIntersectionPoint = aRay.GetOrigin() + aRay.GetDirection() * t;

		return true;
	}

	template<typename T>
	inline bool IntersectionSphereAABB(const Sphere<T>& aSphere, const AABB3D<T>& aAABB3D, Vector3<T>& outIntersectionPoint)
	{
		const auto Clamp = [](T aValue, T aMin, T aMax) { return (aValue < aMin) ? aMin : ((aValue > aMax) ? aMax : aValue); };

		Vector3<T> aabbCenter = aAABB3D.GetMin() + (aAABB3D.GetMax() / T{2});
		Vector3<T> dir = Vector3<T>::Direction(aabbCenter, aSphere.GetCenter());

		Vector3<T> pointOnEdge
		{
			Clamp(aSphere.GetCenter().x, aAABB3D.GetMin().x, aAABB3D.GetMax().x),
			Clamp(aSphere.GetCenter().y, aAABB3D.GetMin().y, aAABB3D.GetMax().y),
			Clamp(aSphere.GetCenter().z, aAABB3D.GetMin().z, aAABB3D.GetMax().z)
		};

		if (pointOnEdge == aSphere.GetCenter())
		{
			// sphere center is inside aabb

			const T x = static_cast<T>(std::abs(dir.x));
			const T y = static_cast<T>(std::abs(dir.y));
			const T z = static_cast<T>(std::abs(dir.z));

			if (x > y && x > z)
			{
				pointOnEdge.x = (pointOnEdge.x > 0) ? aAABB3D.GetMax().x : aAABB3D.GetMin().x;
			}
			else if (y > x && y > z)
			{
				pointOnEdge.y = (pointOnEdge.y > 0) ? aAABB3D.GetMax().y : aAABB3D.GetMin().y;
			}
			else
			{
				pointOnEdge.z = (pointOnEdge.z > 0) ? aAABB3D.GetMax().z : aAABB3D.GetMin().z;
			}

			outIntersectionPoint = pointOnEdge;

			return true;
		}

		Vector3<T> normal = Vector3<T>::Direction(pointOnEdge, aSphere.GetCenter());
		T distance = normal.LengthSqr();

		if (distance <= aSphere.GetRadiusSqr())
		{
			outIntersectionPoint = pointOnEdge;
			return true;
		}

		return false;
	}

	template<typename T>
	inline bool IntersectionAABBRay(const AABB3D<T>& aAABB3D, const Ray<T>& aRay)
	{
		//if (aAABB3D.IsInside(aRay.GetOrigin()))
		//{
		//	return true;
		//}

		//const std::array<Plane<T>, 6> planes
		//{
		//	Plane<T>(aAABB3D.GetMin(), Vector3<T>(-1,  0,  0)),
		//	Plane<T>(aAABB3D.GetMin(), Vector3<T>( 0, -1,  0)),
		//	Plane<T>(aAABB3D.GetMin(), Vector3<T>( 0,  0, -1)),
		//	Plane<T>(aAABB3D.GetMax(), Vector3<T>( 1,  0,  0)),
		//	Plane<T>(aAABB3D.GetMax(), Vector3<T>( 0,  1,  0)),
		//	Plane<T>(aAABB3D.GetMax(), Vector3<T>( 0,  0,  1)),
		//};

		//for (const Plane<T>& plane : planes)
		//{
		//	Vector3<T> intersection;
		//	if (IntersectionPlaneRay(plane, aRay, intersection))
		//	{
		//		if (aAABB3D.IsInside(intersection))
		//		{
		//			return true;
		//		}
		//	}
		//}

		//bool inside = true;

		//T xt{};
		//if (aRay.GetOrigin().x < aAABB3D.GetMin().x)
		//{
		//	xt = aAABB3D.GetMin().x - aRay.GetOrigin().x;
		//	inside = false;
		//}
		//else if (aRay.GetOrigin().x > aAABB3D.GetMax().x)
		//{
		//	xt = aAABB3D.GetMax().x - aRay.GetOrigin().x;
		//	inside = false;
		//}
		//else
		//{
		//	xt = -1;
		//}

		//T yt{};
		//if (aRay.GetOrigin().y < aAABB3D.GetMin().y)
		//{
		//	yt = aAABB3D.GetMin().y - aRay.GetOrigin().y;
		//	inside = false;
		//}
		//else if (aRay.GetOrigin().y > aAABB3D.GetMax().y)
		//{
		//	yt = aAABB3D.GetMax().y - aRay.GetOrigin().y;
		//	inside = false;
		//}
		//else
		//{
		//	yt = -1;
		//}

		//T zt{};
		//if (aRay.GetOrigin().z < aAABB3D.GetMin().z)
		//{
		//	zt = aAABB3D.GetMin().z - aRay.GetOrigin().z;
		//	inside = false;
		//}
		//else if (aRay.GetOrigin().z > aAABB3D.GetMax().z)
		//{
		//	zt = aAABB3D.GetMax().z - aRay.GetOrigin().z;
		//	inside = false;
		//}
		//else
		//{
		//	zt = -1;
		//}

		//if (xt != 0 && aRay.GetDirection().x != 0)
		//{
		//	xt /= aRay.GetDirection().x;
		//}
		//if (yt != 0 && aRay.GetDirection().y != 0)
		//{
		//	yt /= aRay.GetDirection().y;
		//}
		//if (zt != 0 && aRay.GetDirection().z != 0)
		//{
		//	zt /= aRay.GetDirection().z;
		//}

		//if (inside)
		//{
		//	return true;
		//}

		if (aAABB3D.IsInside(aRay.GetOrigin()))
		{
			// for now have this check at start
			return true;
		}

		Vector3<T> t;

		t.x = ((aRay.GetDirection().x > 0 ? aAABB3D.GetMin().x : aAABB3D.GetMax().x) - aRay.GetOrigin().x);
		t.y = ((aRay.GetDirection().y > 0 ? aAABB3D.GetMin().y : aAABB3D.GetMax().y) - aRay.GetOrigin().y);
		t.z = ((aRay.GetDirection().z > 0 ? aAABB3D.GetMin().z : aAABB3D.GetMax().z) - aRay.GetOrigin().z);

		if (aRay.GetDirection().x != 0) t.x /= aRay.GetDirection().x;
		if (aRay.GetDirection().y != 0) t.y /= aRay.GetDirection().y;
		if (aRay.GetDirection().z != 0) t.z /= aRay.GetDirection().z;

		enum class Plane
		{
			YZ,
			XZ,
			XY
		};

		T maxT = t.x;
		Plane whichPlane = Plane::YZ;

		if (t.y > maxT)
		{
			whichPlane = Plane::XZ;
			maxT = t.y;
		}
		if (t.z > maxT)
		{
			whichPlane = Plane::XY;
			maxT = t.z;
		}

		switch (whichPlane)
		{
			case Plane::YZ:
			{
				T y = aRay.GetOrigin().y + aRay.GetDirection().y * maxT;
				if (y < aAABB3D.GetMin().y || y > aAABB3D.GetMax().y)
				{
					return false;
				}

				T z = aRay.GetOrigin().z + aRay.GetDirection().z * maxT;
				if (z < aAABB3D.GetMin().z || z > aAABB3D.GetMax().z)
				{
					return false;
				}

				break;
			}
			case Plane::XZ:
			{
				T x = aRay.GetOrigin().x + aRay.GetDirection().x * maxT;
				if (x < aAABB3D.GetMin().x || x > aAABB3D.GetMax().x)
				{
					return false;
				}

				T z = aRay.GetOrigin().z + aRay.GetDirection().z * maxT;
				if (z < aAABB3D.GetMin().z || z > aAABB3D.GetMax().z)
				{
					return false;
				}

				break;
			}
			case Plane::XY:
			{
				T x = aRay.GetOrigin().x + aRay.GetDirection().x * maxT;
				if (x < aAABB3D.GetMin().x || x > aAABB3D.GetMax().x)
				{
					return false;
				}

				T y = aRay.GetOrigin().y + aRay.GetDirection().y * maxT;
				if (y < aAABB3D.GetMin().y || y > aAABB3D.GetMax().y)
				{
					return false;
				}

				break;
			}
		}

		return true;
	}

	template<typename T>
	inline bool IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		Vector3<T> dir = Vector3<T>::Direction(aRay.GetOrigin(), aSphere.GetCenter());

		if (dir.LengthSqr() <= aSphere.GetRadiusSqr())
		{
			return true;
		}

		T projScalar = dir.Dot(aRay.GetDirection());

		if (projScalar < 0) // pointing away
		{
			return false;
		}

		return aSphere.IsInside(aRay.GetOrigin() + (aRay.GetDirection() * projScalar));
	}
}