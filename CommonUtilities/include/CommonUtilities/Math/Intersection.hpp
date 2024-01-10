#pragma once

#include <cfloat>
#include <cmath>
#include <array>
#include <functional>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>

#include "Plane.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "AABB3D.hpp"
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	struct CollisionResult // TODO: return this instead for additional data
	{
		Vector3<T>	intersection;
		Vector3<T>	normal			{1.0f, 0.0f, 0.0f};	// default normal
		float		penetration		{0.0f};
		bool		collided		{false};

		operator bool() const noexcept
		{
			return collided;
		}
	};

	template<typename T>
	inline CollisionResult<T> IntersectionSphereSphere(const Sphere<T>& aFirstSphere, const Sphere<T>& aSecondSphere);
	
	template<typename T>
	inline CollisionResult<T> IntersectionAABBAABB(const AABB3D<T>& aFirstAABB, const AABB3D<T>& aSecondAABB);

	template<typename T>
	inline CollisionResult<T> IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay);

	template<typename T>
	inline CollisionResult<T> IntersectionSphereAABB(const Sphere<T>& aSphere, const AABB3D<T>& aAABB3D);

	template<typename T>
	inline CollisionResult<T> IntersectionAABBRay(const AABB3D<T>& aAABB3D, const Ray<T>& aRay);

	template<typename T>
	inline CollisionResult<T> IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay);

	template<typename T>
	inline CollisionResult<T> IntersectionSphereSphere(const Sphere<T>& aFirstSphere, const Sphere<T>& aSecondSphere)
	{
		CollisionResult result{};

		Vector3<T> normal = Vector3<T>::Direction(aFirstSphere.GetCenter(), aSecondSphere.GetCenter());

		const T distSqr = normal.LengthSqr();
		const T radius = aFirstSphere.GetRadius() + aSecondSphere.GetRadius();

		if (distSqr >= radius * radius)
		{
			return result;
		}

		if (distSqr > FLT_EPSILON * FLT_EPSILON)
		{
			normal = normal.GetNormalized(std::sqrt(distSqr), 1.0f);
		}

		Vector3<T> firstContact = aFirstSphere.GetCenter() + normal * aFirstSphere.GetRadius();
		Vector3<T> secondContact = aSecondSphere.GetCenter() - normal * aSecondSphere.GetRadius();

		result.intersection = 0.5f * (firstContact + secondContact);
		result.normal		= normal;
		result.penetration	= -Vector3<T>::Direction(firstContact, secondContact).Dot(normal);

		return result;
	}

	template<typename T>
	CollisionResult<T> IntersectionAABBAABB(const AABB3D<T>& aFirstAABB, const AABB3D<T>& aSecondAABB)
	{

		return false;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay)
	{
		CollisionResult result{};

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
			return result;
		}

		result.intersection = aRay.GetOrigin() + aRay.GetDirection() * t;
		result.normal		= aPlane.GetNormal() * -Sign(denom);
		result.penetration	= 0.0f; 
		result.collided		= true;

		return result;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionSphereAABB(const Sphere<T>& aSphere, const AABB3D<T>& aAABB3D)
	{
		CollisionResult result{};

		const auto Clamp = [](T aValue, T aMin, T aMax) { return (aValue < aMin) ? aMin : ((aValue > aMax) ? aMax : aValue); };

		Vector3<T> aabbCenter = aAABB3D.GetMin() + (aAABB3D.GetMax() / T{2});
		Vector3<T> dir = Vector3<T>::Direction(aabbCenter, aSphere.GetCenter());

		Vector3<T> pointOnEdge
		{
			Clamp(aSphere.GetCenter().x, aAABB3D.GetMin().x, aAABB3D.GetMax().x),
			Clamp(aSphere.GetCenter().y, aAABB3D.GetMin().y, aAABB3D.GetMax().y),
			Clamp(aSphere.GetCenter().z, aAABB3D.GetMin().z, aAABB3D.GetMax().z)
		};

		bool inside = false;

		if (pointOnEdge == aSphere.GetCenter())
		{
			// sphere center is inside aabb

			inside = true;

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
		}

		Vector3<T> normal = Vector3<T>::Direction(pointOnEdge, aSphere.GetCenter());
		T distance = normal.LengthSqr();

		if (distance > aSphere.GetRadiusSqr() && !inside)
			return result;

		distance	= std::sqrt(distance);
		normal		= normal.GetNormalized(distance, 1.0f);

		result.normal		= (inside ? -normal : normal);
		result.penetration	= aSphere.GetRadius() - distance;
		result.intersection = pointOnEdge;
		result.collided		= true;

		return result;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionAABBRay(const AABB3D<T>& aAABB3D, const Ray<T>& aRay)
	{
		CollisionResult result{};

		// TODO: handle case when inside

		Vector3<T> t; // get vector from best corner to ray's origin

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
					return result;
				}

				T z = aRay.GetOrigin().z + aRay.GetDirection().z * maxT;
				if (z < aAABB3D.GetMin().z || z > aAABB3D.GetMax().z)
				{
					return result;
				}

				result.normal = Vector3<T>(Sign(aRay.GetDirection().x), 0, 0);

				break;
			}
			case Plane::XZ:
			{
				T x = aRay.GetOrigin().x + aRay.GetDirection().x * maxT;
				if (x < aAABB3D.GetMin().x || x > aAABB3D.GetMax().x)
				{
					return result;
				}

				T z = aRay.GetOrigin().z + aRay.GetDirection().z * maxT;
				if (z < aAABB3D.GetMin().z || z > aAABB3D.GetMax().z)
				{
					return result;
				}

				result.normal = Vector3<T>(0, Sign(aRay.GetDirection().y), 0);

				break;
			}
			case Plane::XY:
			{
				T x = aRay.GetOrigin().x + aRay.GetDirection().x * maxT;
				if (x < aAABB3D.GetMin().x || x > aAABB3D.GetMax().x)
				{
					return result;
				}

				T y = aRay.GetOrigin().y + aRay.GetDirection().y * maxT;
				if (y < aAABB3D.GetMin().y || y > aAABB3D.GetMax().y)
				{
					return result;
				}

				result.normal = Vector3<T>(0, 0, Sign(aRay.GetDirection().z));

				break;
			}
		}

		result.intersection = aRay.GetOrigin() + aRay.GetDirection() * maxT;
		result.penetration	= 0.0f;
		result.collided		= true;

		return result;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		CollisionResult result{};

		Vector3<T> dir = Vector3<T>::Direction(aRay.GetOrigin(), aSphere.GetCenter());

		T distSqr = dir.LengthSqr() - aSphere.GetRadiusSqr();

		if (distSqr > 0) // outside sphere
		{
			return result;
		}

		T projScalar =  dir.Dot(aRay.GetDirection());

		if (projScalar < 0) // pointing away
		{
			return result;
		}

		T discr = projScalar * projScalar - distSqr;

		if (discr < 0) // closest point is outside
		{
			return result;
		}

		T t = -distSqr - std::sqrt(discr);

		if (t < 0) t = 0; // clamp t to zero if inside sphere

		result.intersection = aRay.GetOrigin() + aRay.GetDirection() * t;
		result.normal		= Vector3<T>::Direction(aSphere.GetCenter(), result.intersection)).GetNormalized();
		result.penetration	= 0.0f;
		result.collided		= true;

		return result;
	}
}