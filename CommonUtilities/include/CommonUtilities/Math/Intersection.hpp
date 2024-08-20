#pragma once

#define NOMINMAX

#include <cfloat>
#include <cmath>
#include <array>
#include <functional>
#include <cassert>
#include <stdexcept>
#include <numeric>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>

#include "Plane.hpp"
#include "Ray.hpp"
#include "Sphere.hpp"
#include "AABB.hpp"
#include "Capsule.hpp"
#include "Triangle.hpp"
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	struct ISect 
	{
		Vector3<T>	intersection;						// intersection point of entry time
		Vector3<T>	normal			{1.0f, 0.0f, 0.0f};	// default normal points right
		union
		{
			T		penetration		{0};				// depth intersecting the other shape
			T		enter;								// used only for ray/segment
		};
		T			exit			{0};				// exit time of ray/segment
		bool		intersects		{false};
		bool		inside			{false};			// not always used

		operator bool() const noexcept // implicitly convertible to boolean
		{
			return intersects;
		}
	};

	template<typename T>
	constexpr ISect<T> IntersectionSphereSphere(const Sphere<T>& aFirstSphere, const Sphere<T>& aSecondSphere);

	template<typename T>
	constexpr ISect<T> IntersectionAABBAABB(const AABB<T>& aFirstAABB, const AABB<T>& aSecondAABB);

	template<typename T>
	constexpr ISect<T> IntersectionCapsuleCapsule(const Capsule<T>& aFirstCapsule, const Capsule<T>& aSecondCapsule);

	template<typename T>
	constexpr ISect<T> IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay);

	template<typename T>
	constexpr ISect<T> IntersectionSphereAABB(const Sphere<T>& aSphere, const AABB<T>& aAABB);

	template<typename T>
	constexpr ISect<T> IntersectionAABBRay(const AABB<T>& aAABB, const Ray<T>& aRay);

	template<typename T>
	constexpr ISect<T> IntersectionAABBPlane(const AABB<T>& aAABB, const Plane<T>& aPlane);

	template<typename T>
	constexpr ISect<T> IntersectionCapsuleAABB(const Capsule<T>& aCapsule, const AABB<T>& aAABB);

	template<typename T>
	constexpr ISect<T> IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay);

	template<typename T>
	constexpr ISect<T> IntersectionSpherePlane(const Sphere<T>& aSphere, const Plane<T>& aPlane);

	template<typename T>
	constexpr ISect<T> IntersectionSphereCapsule(const Sphere<T>& aSphere, const Capsule<T>& aCapsule);

	template<typename T>
	constexpr ISect<T> IntersectionCapsulePlane(const Capsule<T>& aCapsule, const Plane<T>& aPlane);

	template<typename T>
	constexpr ISect<T> IntersectionCapsuleRay(const Capsule<T>& aCapsule, const Ray<T>& aRay);

	template<typename T>
	constexpr ISect<T> IntersectionAABBSegment(const AABB<T>& aAABB, const Vector3<T>& aStart, const Vector3<T>& aEnd);

	template<typename T>
	constexpr ISect<T> IntersectionCapsuleSegment(const Capsule<T>& aCapsule, const Vector3<T>& aStart, const Vector3<T>& aEnd);

	template<typename T>
	constexpr ISect<T> IntersectionSphereSegment(const Sphere<T>& aSphere, const Vector3<T>& aStart, const Vector3<T>& aEnd);

	namespace details // hide this from client
	{
		template<typename T>
		constexpr const T& DownCastTo(const Shape& aShape, Shape::Type aExpectedType)
		{
			assert(aShape.GetType() == aExpectedType && "This shape's type is incorrectly set");
			return reinterpret_cast<const T&>(aShape); // should only "crash" if shape has incorrectly set its enum type
		}

		template<typename T>
		constexpr ISect<T> AABBAABB(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionAABBAABB<T>(
				DownCastTo<AABB<T>>(aS1, Shape::Type::AABB), 
				DownCastTo<AABB<T>>(aS2, Shape::Type::AABB));
		}

		template<typename T>
		constexpr ISect<T> SphereSphere(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereSphere<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere), 
				DownCastTo<Sphere<T>>(aS2, Shape::Type::Sphere));
		}

		template<typename T>
		constexpr ISect<T> CapsuleCapsule(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionCapsuleCapsule<T>(
				DownCastTo<Capsule<T>>(aS1, Shape::Type::Capsule),
				DownCastTo<Capsule<T>>(aS2, Shape::Type::Capsule));
		}

		template<typename T>
		constexpr ISect<T> PlaneRay(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionPlaneRay<T>(
				DownCastTo<Plane<T>>(aS1, Shape::Type::Plane),
				DownCastTo<Ray<T>>(aS2, Shape::Type::Ray));
		}
		template<typename T>
		constexpr ISect<T> RayPlane(const Shape& aS1, const Shape& aS2)
		{
			return PlaneRay<T>(aS2, aS1);
		}

		template<typename T>
		constexpr ISect<T> SphereAABB(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereAABB<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere),
				DownCastTo<AABB<T>>(aS2, Shape::Type::AABB));
		}
		template<typename T>
		constexpr ISect<T> AABBSphere(const Shape& aS1, const Shape& aS2)
		{
			ISect<T> result = SphereAABB<T>(aS2, aS1);
			result.normal = -result.normal; // flip normal ??

			return result;
		}

		template<typename T>
		constexpr ISect<T> AABBRay(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionAABBRay<T>(
				DownCastTo<AABB<T>>(aS1, Shape::Type::AABB),
				DownCastTo<Ray<T>>(aS2, Shape::Type::Ray));
		}
		template<typename T>
		constexpr ISect<T> RayAABB(const Shape& aS1, const Shape& aS2)
		{
			return AABBRay<T>(aS2, aS1);
		}

		template<typename T>
		constexpr ISect<T> AABBPlane(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionAABBPlane<T>(
				DownCastTo<AABB<T>>(aS1, Shape::Type::AABB),
				DownCastTo<Plane<T>>(aS2, Shape::Type::Plane));
		}
		template<typename T>
		constexpr ISect<T> PlaneAABB(const Shape& aS1, const Shape& aS2)
		{
			return AABBPlane<T>(aS2, aS1);
		}

		template<typename T>
		constexpr ISect<T> CapsuleAABB(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionCapsuleAABB<T>(
				DownCastTo<Capsule<T>>(aS1, Shape::Type::Capsule),
				DownCastTo<AABB<T>>(aS2, Shape::Type::AABB));
		}
		template<typename T>
		constexpr ISect<T> AABBCapsule(const Shape& aS1, const Shape& aS2)
		{
			return CapsuleAABB<T>(aS2, aS1);
		}

		template<typename T>
		constexpr ISect<T> SphereRay(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereRay<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere),
				DownCastTo<Ray<T>>(aS2, Shape::Type::Ray));
		}
		template<typename T>
		constexpr ISect<T> RaySphere(const Shape& aS1, const Shape& aS2)
		{
			return SphereRay<T>(aS2, aS1);
		}

		template<typename T>
		constexpr ISect<T> SpherePlane(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSpherePlane<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere),
				DownCastTo<Plane<T>>(aS2, Shape::Type::Plane));
		}
		template<typename T>
		constexpr ISect<T> PlaneSphere(const Shape& aS1, const Shape& aS2)
		{
			return SpherePlane<T>(aS2, aS1);
		}

		template<typename T>
		constexpr ISect<T> SphereCapsule(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereCapsule<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere),
				DownCastTo<Capsule<T>>(aS2, Shape::Type::Capsule));
		}
		template<typename T>
		constexpr ISect<T> CapsuleSphere(const Shape& aS1, const Shape& aS2)
		{
			return SphereCapsule<T>(aS2, aS1);
		}

		template<typename T>
		constexpr ISect<T> CapsulePlane(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereCapsule<T>(
				DownCastTo<Capsule<T>>(aS1, Shape::Type::Capsule),
				DownCastTo<Plane<T>>(aS2, Shape::Type::Plane));
		}
		template<typename T>
		constexpr ISect<T> PlaneCapsule(const Shape& aS1, const Shape& aS2)
		{
			return CapsulePlane<T>(aS2, aS1);
		}

		template<typename T>
		constexpr ISect<T> CapsuleRay(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionCapsuleRay<T>(
				DownCastTo<Capsule<T>>(aS1, Shape::Type::Capsule),
				DownCastTo<Ray<T>>(aS2, Shape::Type::Ray));
		}
		template<typename T>
		constexpr ISect<T> RayCapsule(const Shape& aS1, const Shape& aS2)
		{
			return CapsuleRay<T>(aS2, aS1);
		}

		///		ab	sp	cu	ry	pl	tr
		/// ab |-X-|-X-|-X-|-X-|-X-|---|
		/// sp |-X-|-X-|-X-|-X-|-X-|---|
		/// li |-X-|-X-|-X-|-X-|-X-|---|
		/// lv |-X-|-X-|-X-|---|-X-|---|
		/// pl |-X-|-X-|-X-|-X-|---|---|
		///	tr |---|---|---|---|---|---|
		/// 
		template<typename T>
		inline static std::array<ISect<T>(*)(const Shape&, const Shape&),
			static_cast<int>(Shape::Type::Count) * static_cast<int>(Shape::Type::Count)> globalCollisionMatrix
		{
			// # aabb			# sphere			# capsule				# ray				# plane				# triangle
			AABBAABB<T>,		AABBSphere<T>,		AABBCapsule<T>,			AABBRay<T>,			AABBPlane<T>,		nullptr,		nullptr,		nullptr,	nullptr,
			SphereAABB<T>,		SphereSphere<T>,	SphereCapsule<T>,		SphereRay<T>,		SpherePlane<T>,		nullptr,		nullptr,		nullptr,	nullptr,
			CapsuleAABB<T>,		CapsuleSphere<T>,	CapsuleCapsule<T>,		CapsuleRay<T>,		CapsulePlane<T>,	nullptr,		nullptr,		nullptr,	nullptr,
			RayAABB<T>,			RaySphere<T>,		RayCapsule<T>,			nullptr,			RayPlane<T>,		nullptr,		nullptr,		nullptr,	nullptr,
			PlaneAABB<T>,		PlaneSphere<T>,		PlaneCapsule<T>,		PlaneRay<T>,		nullptr,			nullptr,		nullptr,		nullptr,	nullptr,
			nullptr,			nullptr,			nullptr,				nullptr,			nullptr,			nullptr,		nullptr,		nullptr,	nullptr,
			nullptr,			nullptr,			nullptr,				nullptr,			nullptr,			nullptr,		nullptr,		nullptr,	nullptr,
			nullptr,			nullptr,			nullptr,				nullptr,			nullptr,			nullptr,		nullptr,		nullptr,	nullptr,
			nullptr,			nullptr,			nullptr,				nullptr,			nullptr,			nullptr,		nullptr,		nullptr,	nullptr
		};
	}

	/// Collide a generic shape against another generic shape. If you know both types already, you should
	/// call the corresponding collision detection algorithm directly instead. Furthermore, make sure that
	/// the shapes arithmetic template type is the same when compared with each other, and also the type
	/// provided to this function. Otherwise, undefined behaviour WILL happen.
	/// 
	/// \param aFirstShape: First shape to collide against
	/// \param aSecondShape: Second shape to collide against
	/// 
	/// \return Result from collision
	/// 
	template<typename T>
	constexpr ISect<T> Collide(const Shape& aFirstShape, const Shape& aSecondShape)
	{
		const int collisionIndex = static_cast<int>(aSecondShape.GetType()) +
			static_cast<int>(aFirstShape.GetType()) * static_cast<int>(Shape::Type::Count);

		if (collisionIndex < 0 || collisionIndex >= details::globalCollisionMatrix<T>.size())
		{
			throw std::out_of_range("Collision for these two shapes is out of range of the matrix!");
		}

		const auto collisionFuncPtr = details::globalCollisionMatrix<T>[collisionIndex];

		if (collisionFuncPtr == nullptr)
		{
			throw std::runtime_error("No collision algorithm is defined for these two shapes!");
		}

		return collisionFuncPtr(aFirstShape, aSecondShape);
	}

	template<typename T>
	constexpr ISect<T> IntersectionSphereSphere(const Sphere<T>& aFirstSphere, const Sphere<T>& aSecondSphere)
	{
		ISect<T> result{};

		Vector3<T> normal = Vector3<T>::Direction(aFirstSphere.GetCenter(), aSecondSphere.GetCenter());

		const T distSqr = normal.LengthSqr();
		const T radius = aFirstSphere.GetRadius() + aSecondSphere.GetRadius();

		if (distSqr >= radius * radius)
		{
			return result;
		}

		if (!Equal(distSqr, T(0), EPSILON_V<T> *EPSILON_V<T>))
		{
			normal = normal.GetNormalized(std::sqrt(distSqr), 1.0f);
		}

		Vector3<T> firstContact = aFirstSphere.GetCenter() + normal * aFirstSphere.GetRadius();
		Vector3<T> secondContact = aSecondSphere.GetCenter() - normal * aSecondSphere.GetRadius();

		result.intersection = 0.5f * (firstContact + secondContact);
		result.normal		= (normal == Vector3<T>()) ? Vector3<T>(1, 0, 0) : normal;
		result.penetration	= -Vector3<T>::Direction(firstContact, secondContact).Dot(normal);
		result.intersects		= true;

		return result;
	}

	template<typename T>
	constexpr ISect<T> IntersectionAABBAABB(const AABB<T>& aFirstAABB, const AABB<T>& aSecondAABB)
	{
		ISect<T> result{};

		Vector3<T> dir = Vector3<T>::Direction(aFirstAABB.GetCenter(), aSecondAABB.GetCenter());

		Vector3<T> aExtends = aFirstAABB.GetSize() / 2.0f;
		Vector3<T> bExtends = aSecondAABB.GetSize() / 2.0f;

		// we use a simplified SAT test

		if (T xOverlap = (aExtends.x + bExtends.x - std::abs(dir.x)); xOverlap > 0)
		{
			if (T yOverlap = (aExtends.y + bExtends.y - std::abs(dir.y)); yOverlap > 0)
			{
				if (T zOverlap = (aExtends.z + bExtends.z - std::abs(dir.z)); zOverlap > 0)
				{
					// find axis of least penetration
					if (xOverlap < yOverlap && xOverlap < zOverlap) // x-axis
					{
						result.normal		= (dir.x < 0) ? Vector3<T>(1, 0, 0) : Vector3<T>(-1, 0, 0);
						result.penetration	= xOverlap;
						result.intersects	= true;
					}
					else if (yOverlap < xOverlap && yOverlap < zOverlap) // y-axis
					{
						result.normal		= (dir.y < 0) ? Vector3<T>(0, 1, 0) : Vector3<T>(0, -1, 0);
						result.penetration	= yOverlap;
						result.intersects	= true;
					}
					else // z-axis
					{
						result.normal		= (dir.z < 0) ? Vector3<T>(0, 0, 1) : Vector3<T>(0, 0, -1);
						result.penetration	= zOverlap;
						result.intersects	= true;
					}
				}
				//else // 2D collision
				//{
				//	if (xOverlap < yOverlap) // x-axis
				//	{
				//		result.normal = (dir.x < 0) ? Vector3<T>(1, 0, 0) : Vector3<T>(-1, 0, 0);
				//		result.penetration = xOverlap;
				//		result.intersects = true;
				//	}
				//	else  // y-axis
				//	{
				//		result.normal = (dir.y < 0) ? Vector3<T>(0, 1, 0) : Vector3<T>(0, -1, 0);
				//		result.penetration = yOverlap;
				//		result.intersects = true;
				//	}
				//}
			}
		}

		return result;
	}

	template<typename T>
	constexpr ISect<T> IntersectionCapsuleCapsule(const Capsule<T>& aFirstCapsule, const Capsule<T>& aSecondCapsule)
	{
		const auto [p1, p2] = Vector3<T>::ClosestPointsSegmentSegment(aFirstCapsule.GetBase(), aFirstCapsule.GetTip(), aSecondCapsule.GetBase(), aSecondCapsule.GetTip());
		return IntersectionSphereSphere(Sphere<T>(p1, aFirstCapsule.GetRadius()), Sphere<T>(p2, aSecondCapsule.GetRadius()));
	}

	template<typename T>
	constexpr ISect<T> IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay)
	{
		ISect<T> result{};

		T numen = Vector3<T>::Direction(aRay.GetOrigin(), aPlane.GetOrigin()).Dot(aPlane.GetNormal());
		T denom = aPlane.GetNormal().Dot(aRay.GetDirection());

		if (Equal<T>(denom, 0))
		{
			// if the numenator is zero then the origin of the ray lies on plane, which it must if parallel to the plane and cause intersection
			if (numen == 0)
			{
				result.intersection = aRay.GetOrigin();
				result.normal		= aPlane.GetNormal();
				result.intersects	= true;
			}

			return result;
		}

		T t = numen / denom;

		if (t < 0) // ray points away from plane
		{
			return result;
		}

		result.intersection = aRay.GetOrigin() + aRay.GetDirection() * t;
		result.normal		= aPlane.GetNormal() * -Sign<T>(denom); // flip normal based on what side we are approaching from
		result.enter		= t;
		result.exit			= t;
		result.intersects	= true;

		return result;
	}

	template<typename T>
	constexpr ISect<T> IntersectionSphereAABB(const Sphere<T>& aSphere, const AABB<T>& aAABB)
	{
		ISect<T> result{};

		const auto Clamp = [](T aValue, T aMin, T aMax) { return (aValue < aMin) ? aMin : ((aValue > aMax) ? aMax : aValue); };

		const Vector3<T> aabbCenter = aAABB.GetCenter();
		const Vector3<T> dir = Vector3<T>::Direction(aabbCenter, aSphere.GetCenter());

		Vector3<T> pointOnEdge
		{
			Clamp(aSphere.GetCenter().x, aAABB.GetMin().x, aAABB.GetMax().x),
			Clamp(aSphere.GetCenter().y, aAABB.GetMin().y, aAABB.GetMax().y),
			Clamp(aSphere.GetCenter().z, aAABB.GetMin().z, aAABB.GetMax().z)
		};

		bool inside = false;

		if (pointOnEdge == aSphere.GetCenter())
		{
			// sphere center is inside aabb

			inside = true;

			const T x = T(std::abs(dir.x));
			const T y = T(std::abs(dir.y));
			const T z = T(std::abs(dir.z));

			if (x > y && x > z)
			{
				pointOnEdge.x = (pointOnEdge.x > 0) ? aAABB.GetMax().x : aAABB.GetMin().x;
			}
			else if (y > x && y > z)
			{
				pointOnEdge.y = (pointOnEdge.y > 0) ? aAABB.GetMax().y : aAABB.GetMin().y;
			}
			else
			{
				pointOnEdge.z = (pointOnEdge.z > 0) ? aAABB.GetMax().z : aAABB.GetMin().z;
			}
		}

		Vector3<T> normal = Vector3<T>::Direction(pointOnEdge, aSphere.GetCenter());
		T distance = normal.LengthSqr();

		if (distance > aSphere.GetRadiusSqr() && !inside)
			return result;

		if (!Equal(distance, T(0), EPSILON_V<T> * EPSILON_V<T>))
		{ 
			distance	= std::sqrt(distance);
			normal		= normal.GetNormalized(distance, 1.0f);
		}

		result.intersection = pointOnEdge;
		result.normal		= (inside ? -normal : normal);
		result.penetration	= aSphere.GetRadius() - distance;
		result.intersects	= true;

		return result;
	}

	template<typename T>
	constexpr ISect<T> IntersectionAABBRay(const AABB<T>& aAABB, const Ray<T>& aRay)
	{
		ISect<T> result{};

		const Vector3<T> t1 = (aAABB.GetMin() - aRay.GetOrigin()) / aRay.GetDirection();
		const Vector3<T> t2 = (aAABB.GetMax() - aRay.GetOrigin()) / aRay.GetDirection();

		T tMin = 0;
		T tMax = MAX_V<T>;

		tMin = Min(Max(t1.x, tMin), Max(t2.x, tMin));
		tMax = Max(Min(t1.x, tMax), Min(t2.x, tMax));

		tMin = Min(Max(t1.y, tMin), Max(t2.y, tMin));
		tMax = Max(Min(t1.y, tMax), Min(t2.y, tMax));

		tMin = Min(Max(t1.z, tMin), Max(t2.z, tMin));
		tMax = Max(Min(t1.z, tMax), Min(t2.z, tMax));

		if (tMin <= tMax)
		{ 
			bool inside = (tMin <= T(0));

			tMin = Max(tMin, T(0)); // clamp tMin to zero if inside aabb

			result.intersection = aRay.GetOrigin() + aRay.GetDirection() * tMin;
			result.enter		= tMin;
			result.exit			= tMax;
			result.inside		= inside;
			result.intersects	= true;

			const Vector3<T> dir = Vector3<T>::Direction(aAABB.GetCenter(), result.intersection);

			const T x = T(std::abs(dir.x));
			const T y = T(std::abs(dir.y));
			const T z = T(std::abs(dir.z));

			if (x > y && x > z)
			{
				result.normal = { Sign(dir.x), T(0), T(0) };
			}
			else if (y > x && y > z)
			{
				result.normal = { T(0), Sign(dir.y), T(0) };
			}
			else
			{
				result.normal = { T(0), T(0), Sign(dir.z) };
			}
		}

		return result;
	}

	template<typename T>
	constexpr ISect<T> IntersectionAABBPlane(const AABB<T>& aAABB, const Plane<T>& aPlane)
	{
		return ISect<T>();
	}

	template<typename T>
	constexpr ISect<T> IntersectionCapsuleAABB(const Capsule<T>& aCapsule, const AABB<T>& aAABB)
	{
		ISect<T> result{};

		const ISect<T> isectSegAABB = IntersectionAABBSegment(aAABB, aCapsule.GetBase(), aCapsule.GetTip());

		const std::array<Vector3<T>, 8> points = aAABB.GetPoints();
		
		if (isectSegAABB) // capsule is deep
		{
			const Vector3<T> dir = Vector3<T>::Direction(aCapsule.GetBase(), aCapsule.GetTip());

			const Vector3<T> innerStart = dir + aCapsule.GetBase() * isectSegAABB.enter;
			const Vector3<T> innerEnd	= dir + aCapsule.GetBase() * isectSegAABB.exit;

			Vector3<T> farthestPointOnSegment;
			T largestDistance = MIN_V<T>;

			for (std::size_t i = 0; i < points.size(); ++i)
			{
				const Vector3<T> pointOnSegment = Vector3<T>::ClosestPointOnSegment(innerStart, innerEnd, points[i]);
				const Vector3<T> dir = Vector3<T>::Direction(pointOnSegment, points[i]);

				if (const T lengthSqr = dir.LengthSqr(); lengthSqr > largestDistance)
				{
					farthestPointOnSegment = pointOnSegment;
					largestDistance = lengthSqr;
				}
			}

			return IntersectionSphereAABB(Sphere<T>(farthestPointOnSegment, aCapsule.GetRadius()), aAABB);
		}

		// capsule is shallow

		Vector3<T> closestPointOnSegment;
		T smallestDistance = MAX_V<T>;

		for (std::size_t i = 0; i < points.size(); ++i)
		{
			const Vector3<T> pointOnSegment = Vector3<T>::ClosestPointOnSegment(aCapsule.GetBase(), aCapsule.GetTip(), points[i]);
			const Vector3<T> dir			= Vector3<T>::Direction(pointOnSegment, points[i]);

			if (const T lengthSqr = dir.LengthSqr(); lengthSqr < smallestDistance)
			{
				closestPointOnSegment = pointOnSegment;
				smallestDistance = lengthSqr;
			}
		}

		return IntersectionSphereAABB(Sphere<T>(closestPointOnSegment, aCapsule.GetRadius()), aAABB);

		//cu::AABB<T> expandedAABB
		//{
		//	aAABB.GetMin() - Vector3<T>(aCapsule.GetRadius()),
		//	aAABB.GetMin() + Vector3<T>(aCapsule.GetRadius())
		//};

		//const ISect<T> isectSegAABB = IntersectionAABBSegment(expandedAABB, aCapsule.GetBase(), aCapsule.GetTip());
		//
		//if (!isectSegAABB)
		//	return result;

		//const Vector3<T> AB = Vector3<T>::Direction(aCapsule.GetBase(), aCapsule.GetTip());

		//const Vector3<T> p1 = isectSegAABB.intersection;
		//const Vector3<T> p2 = aCapsule.GetBase() + AB * isectSegAABB.exit;

		//int u = 0, v = 0;
		//if (p1.x < aAABB.min.x) u |= 1;
		//if (p1.x > aAABB.max.x) v |= 1;
		//if (p1.y < aAABB.min.y) u |= 2;
		//if (p1.y > aAABB.max.y) v |= 2;
		//if (p1.z < aAABB.min.z) u |= 4;
		//if (p1.z > aAABB.max.z) v |= 4;

		//const auto GetCorner =
		//	[&aAABB](int aIndex) -> Vector3<T>
		//	{
		//		Vector3<T> p;
		//		p.x = ((aIndex & 1) ? aAABB.GetMax().x, aAABB.GetMin().x);
		//		p.y = ((aIndex & 2) ? aAABB.GetMax().y, aAABB.GetMin().y);
		//		p.z = ((aIndex & 4) ? aAABB.GetMax().z, aAABB.GetMin().z);
		//		return p;
		//	};

		//int m = u | v;

		//if (m == 7)
		//{

		//}

		//if ((m & (m - 1)) == 0) // in face region
		//{
		//	result.intersection = isectSegAABB.intersection;
		//	result.normal		= isectSegAABB.normal;
		//	result.intersects	= isectSegAABB.intersects;
		//	result.inside		= isectSegAABB.inside;
		//}
	}

	template<typename T>
	constexpr ISect<T> IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		ISect<T> result{};

		const Vector3<T> dir = Vector3<T>::Direction(aRay.GetOrigin(), aSphere.GetCenter());

		const T distSqr = dir.LengthSqr() - aSphere.GetRadiusSqr();
		const T projScalar = dir.Dot(aRay.GetDirection());

		if (distSqr > 0 && projScalar < 0) // outside sphere and pointing away
			return result;

		T discr = projScalar * projScalar - distSqr;

		if (discr < 0) // closest point is outside
			return result;

		discr = std::sqrt(discr);

		T t1 = projScalar - discr;
		T t2 = projScalar + discr;

		if (t1 > t2) (std::swap)(t1, t2);

		bool inside = (t1 < T(0));

		t1 = Max(t1, T(0)); // clamp t to zero if inside sphere

		result.intersection = aRay.GetOrigin() + aRay.GetDirection() * t1;
		result.normal		= Vector3<T>::Direction(aSphere.GetCenter(), result.intersection).GetNormalized();
		result.enter		= t1;
		result.exit			= t2;
		result.intersects	= true;
		result.inside		= inside;

		return result;
	}

	template<typename T>
	constexpr ISect<T> IntersectionSpherePlane(const Sphere<T>& aSphere, const Plane<T>& aPlane)
	{
		return ISect<T>();
	}

	template<typename T>
	constexpr ISect<T> IntersectionSphereCapsule(const Sphere<T>& aSphere, const Capsule<T>& aCapsule)
	{
		const Vector3<T> p = Vector3<T>::ClosestPointOnSegment(aCapsule.GetBase(), aCapsule.GetTip(), aSphere.GetCenter());
		return IntersectionSphereSphere(aSphere, Sphere<T>(p, aCapsule.GetRadius()));
	}

	template<typename T>
	constexpr ISect<T> IntersectionCapsulePlane(const Capsule<T>& aCapsule, const Plane<T>& aPlane)
	{
		return ISect<T>();
	}

	template<typename T>
	constexpr ISect<T> IntersectionCapsuleRay(const Capsule<T>& aCapsule, const Ray<T>& aRay)
	{
		const Vector3<T> AB = Vector3<T>::Direction(aCapsule.GetBase(), aCapsule.GetTip());
		const Vector3<T> AO = Vector3<T>::Direction(aCapsule.GetBase(), aRay.GetOrigin());

		const T ABdotD	= Vector3<T>::Dot(AB, aRay.GetDirection());
		const T ABdotAO = Vector3<T>::Dot(AB, AO);
		const T ABdotAB = Vector3<T>::Dot(AB, AB);

		const T m = ABdotD / ABdotAB;
		const T n = ABdotAO / ABdotAB;

		const Vector3<T> Q = aRay.GetDirection() - (AB * m);
		const Vector3<T> R = AO - (AB * n);

		// https://gist.github.com/jdryg/ecde24d34aa0ce2d4d87 ...

		return ISect<T>(); // TODO: implement
	}

	template<typename T>
	constexpr ISect<T> IntersectionAABBSegment(const AABB<T>& aAABB, const Vector3<T>& aStart, const Vector3<T>& aEnd)
	{
		ISect<T> result{};

		const Vector3<T> dir = Vector3<T>::Direction(aStart, aEnd);

		const Vector3<T> t1 = (aAABB.GetMin() - aStart) / dir;
		const Vector3<T> t2 = (aAABB.GetMax() - aStart) / dir;

		T tMin = 0;
		T tMax = MAX_V<T>;

		tMin = Min(Max(t1.x, tMin), Max(t2.x, tMin));
		tMax = Max(Min(t1.x, tMax), Min(t2.x, tMax));

		tMin = Min(Max(t1.y, tMin), Max(t2.y, tMin));
		tMax = Max(Min(t1.y, tMax), Min(t2.y, tMax));

		tMin = Min(Max(t1.z, tMin), Max(t2.z, tMin));
		tMax = Max(Min(t1.z, tMax), Min(t2.z, tMax));

		if (tMin <= tMax)
		{
			bool inside = (t1 <= T(0) && t2 >= T(1));

			tMin = Max(tMin, T(0)); // clamp tMin to zero if inside aabb
			tMax = Min(tMax, T(1)); // clamp tMax to one if inside aabb

			result.intersection = aStart + dir * tMin;
			result.enter		= tMin;
			result.exit			= tMax;
			result.inside		= inside;
			result.intersects	= true;

			const Vector3<T> dir = Vector3<T>::Direction(aAABB.GetCenter(), result.intersection);

			const T x = T(std::abs(dir.x));
			const T y = T(std::abs(dir.y));
			const T z = T(std::abs(dir.z));

			if (x > y && x > z)
			{
				result.normal = { Sign(dir.x), T(0), T(0) };
			}
			else if (y > x && y > z)
			{
				result.normal = { T(0), Sign(dir.y), T(0) };
			}
			else
			{
				result.normal = { T(0), T(0), Sign(dir.z) };
			}
		}

		return result;
	}

	template<typename T>
	constexpr ISect<T> IntersectionCapsuleSegment(const Capsule<T>& aCapsule, const Vector3<T>& aStart, const Vector3<T>& aEnd)
	{
		ISect<T> result{};

		const Vector3<T> A = aCapsule.GetBase();
		const Vector3<T> B = aCapsule.GetTip();
		const Vector3<T> C = aStart;
		const Vector3<T> D = aEnd;

		const auto CheckSpheres = 
			[&aCapsule, &A, &B, &C, &D]() -> ISect<T>
			{
				ISect<T> isectSphereBaseSeg = IntersectionSphereSegment(Sphere<T>(A, aCapsule.GetRadius()), C, D);

				if (isectSphereBaseSeg.inside)
					return isectSphereBaseSeg;

				ISect<T> isectSphereTipSeg = IntersectionSphereSegment(Sphere<T>(B, aCapsule.GetRadius()), C, D);

				if (isectSphereTipSeg.inside)
					return isectSphereTipSeg;

				if (isectSphereBaseSeg)
				{
					if (isectSphereTipSeg)
					{
						if (isectSphereBaseSeg.enter < isectSphereTipSeg.enter)
						{
							isectSphereBaseSeg.exit = Max(isectSphereBaseSeg.exit, isectSphereTipSeg.exit);
							return isectSphereBaseSeg;
						}
						else
						{
							isectSphereTipSeg.exit = Max(isectSphereBaseSeg.exit, isectSphereTipSeg.exit);
							return isectSphereTipSeg;
						}
					}
					else
						return isectSphereBaseSeg;
				}

				if (isectSphereTipSeg)
					return isectSphereTipSeg;

				return ISect<T>{};
			};

		const Vector3<T> AB = Vector3<T>::Direction(A, B);
		const Vector3<T> AC = Vector3<T>::Direction(A, C);
		const Vector3<T> CD = Vector3<T>::Direction(C, D);

		const T ACdotAB = Vector3<T>::Dot(AC, AB);
		const T CDdotAB = Vector3<T>::Dot(CD, AB);
		const T ABdotAB = Vector3<T>::Dot(AB, AB);

		if (ABdotAB <= EPSILON_V<T>)
			return IntersectionSphereSegment(Sphere<T>((B + A) / T(2), aCapsule.GetRadius()), aStart, aEnd);

		if (ACdotAB < T(0) && ACdotAB + CDdotAB < T(0)) // outside on base side
			return IntersectionSphereSegment(Sphere<T>(A, aCapsule.GetRadius()), aStart, aEnd);

		if (ACdotAB > ABdotAB && ACdotAB + CDdotAB > ABdotAB) // outside on tip side
			return IntersectionSphereSegment(Sphere<T>(B, aCapsule.GetRadius()), aStart, aEnd);

		const T CDdotCD = Vector3<T>::Dot(CD, CD);
		const T ACdotCD = Vector3<T>::Dot(AC, CD);

		const T a = ABdotAB * CDdotCD - CDdotAB * CDdotAB;
		const T k = Vector3<T>::Dot(AC, AC) - aCapsule.GetRadiusSqr();
		const T c = ABdotAB * k - ACdotAB * ACdotAB;

		if (T((std::abs)(a)) < EPSILON_V<T>)
		{
			if (c > T(0)) // outside cylinder
			{
				if (ISect<T> isectSphereBaseSeg = IntersectionSphereSegment(Sphere<T>(A, aCapsule.GetRadius()), aStart, aEnd))
					return isectSphereBaseSeg;

				if (ISect<T> isectSphereTipSeg = IntersectionSphereSegment(Sphere<T>(B, aCapsule.GetRadius()), aStart, aEnd))
					return isectSphereTipSeg;

				return result;
			}

			if (ACdotAB < T(0) || ACdotAB > ABdotAB) // intersects either base or tip from outside
			{
				return CheckSpheres();
			}
			else // completely inside
			{
				result.intersection = aStart;
				result.normal		= Vector3<T>::Direction(A + AB * ACdotAB / ABdotAB, result.intersection).GetNormalized();
				result.enter		= T(0);
				result.exit			= T(1);
				result.intersects	= true;
				result.inside		= true;
			}

			return result;
		}

		const T b = ABdotAB * ACdotCD - CDdotAB * ACdotAB;
		T discr = b * b - a * c;

		if (discr < T(0))
			return result;

		discr = std::sqrt(discr);

		T t1 = (-b - discr) / a;
		T t2 = (-b + discr) / a;

		if (t1 > t2) (std::swap)(t1, t2);

		ISect<T> outsideISect;

		const bool entryOutside = ACdotAB + t1 * CDdotAB < T(0) || ACdotAB + t1 * CDdotAB > ABdotAB;
		const bool exitOutside  = ACdotAB + t2 * CDdotAB < T(0) || ACdotAB + t2 * CDdotAB > ABdotAB;

		if (entryOutside || exitOutside)
		{
			ISect<T> spheresISect = CheckSpheres();

			if (entryOutside)
			{
				outsideISect.intersection	= spheresISect.intersection;
				outsideISect.normal			= spheresISect.normal;
				outsideISect.enter			= spheresISect.enter;
				outsideISect.exit			= Min(t2, T(1));
				outsideISect.intersects		= spheresISect.intersects;
				outsideISect.inside			= spheresISect.inside;

				return outsideISect;
			}

			if (exitOutside)
			{
				outsideISect.exit = spheresISect.exit;
			}
		}

		if (t1 > T(1))
		{
			return CheckSpheres();
		}
		else if (t1 < T(0))
		{
			if (c <= T(0))
			{
				result.intersection = aStart;
				result.normal		= Vector3<T>::Direction(A + AB * ACdotAB / ABdotAB, result.intersection).GetNormalized();
				result.enter		= T(0);
				result.exit			= exitOutside ? outsideISect.exit : Min(t2, T(1));
				result.intersects	= true;
				result.inside		= true;

				return result;
			}

			return CheckSpheres();
		}

		t1 = Max(t1, T(0));

		result.intersection = C + CD * t1;
		result.normal		= Vector3<T>::Direction(Vector3<T>::ClosestPointOnLine(A, B, result.intersection), result.intersection).GetNormalized();
		result.enter		= t1;
		result.exit			= exitOutside ? outsideISect.exit : Min(t2, T(1));
		result.intersects	= true;

		return result;
	}

	template<typename T>
	constexpr ISect<T> IntersectionSphereSegment(const Sphere<T>& aSphere, const Vector3<T>& aStart, const Vector3<T>& aEnd)
	{
		ISect<T> result{};

		const Vector3<T> origin = Vector3<T>::Direction(aStart, aSphere.GetCenter());
		const Vector3<T> dir	= Vector3<T>::Direction(aStart, aEnd);

		const T a = Vector3<T>::Dot(dir, dir);

		if (a == T(0))
			return result;

		const T b = Vector3<T>::Dot(origin, dir);
		const T c = Vector3<T>::Dot(origin, origin) - aSphere.GetRadiusSqr();

		T discr = b * b - a * c;

		if (discr < T(0))
			return result;

		discr = std::sqrt(discr);

		T t1 = (b - discr) / a;
		T t2 = (b + discr) / a;

		if (t1 > t2) (std::swap)(t1, t2);

		bool inside = (t1 <= T(0) && t2 >= T(1));

		t1 = Max(t1, T(0));
		t2 = Min(t2, T(1));

		if (t2 < t1)
			return result;

		result.intersection = aStart + dir * t1;
		result.normal		= (aSphere.GetCenter() != result.intersection) ? Vector3<T>::Direction(aSphere.GetCenter(), result.intersection).GetNormalized() : Vector3<T>::Right;
		result.enter		= t1;
		result.exit			= t2;
		result.intersects	= true;
		result.inside		= inside;

		return result;
	}

	using ISectf = ISect<float>;
	using ISecti = ISect<int>;
	using ISectd = ISect<double>;
}