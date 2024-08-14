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
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	struct CollisionResult 
	{
		Vector3<T>	intersection;
		Vector3<T>	normal			{1.0f, 0.0f, 0.0f};	// default normal points right
		T			penetration		{0};
		T			enter			{0}; // used only for ray
		T			exit			{0};
		bool		collided		{false};

		operator bool() const noexcept // implicitly convertible to boolean
		{
			return collided;
		}
	};

	template<typename T>
	inline CollisionResult<T> IntersectionSphereSphere(const Sphere<T>& aFirstSphere, const Sphere<T>& aSecondSphere);

	template<typename T>
	inline CollisionResult<T> IntersectionAABBAABB(const AABB<T>& aFirstAABB, const AABB<T>& aSecondAABB);

	template<typename T>
	inline CollisionResult<T> IntersectionCapsuleCapsule(const Capsule<T>& aFirstCapsule, const Capsule<T>& aSecondCapsule);

	template<typename T>
	inline CollisionResult<T> IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay);

	template<typename T>
	inline CollisionResult<T> IntersectionSphereAABB(const Sphere<T>& aSphere, const AABB<T>& aAABB);

	template<typename T>
	inline CollisionResult<T> IntersectionAABBRay(const AABB<T>& aAABB, const Ray<T>& aRay);

	template<typename T>
	inline CollisionResult<T> IntersectionAABBPlane(const AABB<T>& aAABB, const Plane<T>& aPlane);

	template<typename T>
	inline CollisionResult<T> IntersectionAABBCapsule(const AABB<T>& aAABB, const Capsule<T>& aCapsule);

	template<typename T>
	inline CollisionResult<T> IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay);

	template<typename T>
	inline CollisionResult<T> IntersectionSpherePlane(const Sphere<T>& aSphere, const Plane<T>& aPlane);

	template<typename T>
	inline CollisionResult<T> IntersectionSphereCapsule(const Sphere<T>& aSphere, const Capsule<T>& aCapsule);

	template<typename T>
	inline CollisionResult<T> IntersectionCapsulePlane(const Capsule<T>& aCapsule, const Plane<T>& aPlane);

	namespace details // hide this from client
	{
		template<typename T>
		inline const T& DownCastTo(const Shape& aShape, Shape::Type aExpectedType)
		{
			assert(aShape.GetType() == aExpectedType && "This shape's type is incorrectly set");
			return reinterpret_cast<const T&>(aShape); // should only "crash" if shape has incorrectly set its enum type
		}

		template<typename T>
		inline CollisionResult<T> AABBAABB(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionAABBAABB<T>(
				DownCastTo<AABB<T>>(aS1, Shape::Type::AABB), 
				DownCastTo<AABB<T>>(aS2, Shape::Type::AABB));
		}

		template<typename T>
		inline CollisionResult<T> SphereSphere(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereSphere<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere), 
				DownCastTo<Sphere<T>>(aS2, Shape::Type::Sphere));
		}

		template<typename T>
		inline CollisionResult<T> CapsuleCapsule(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionCapsuleCapsule<T>(
				DownCastTo<Capsule<T>>(aS1, Shape::Type::Capsule),
				DownCastTo<Capsule<T>>(aS2, Shape::Type::Capsule));
		}

		template<typename T>
		inline CollisionResult<T> PlaneRay(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionPlaneRay<T>(
				DownCastTo<Plane<T>>(aS1, Shape::Type::Plane),
				DownCastTo<Ray<T>>(aS2, Shape::Type::Ray));
		}
		template<typename T>
		inline CollisionResult<T> RayPlane(const Shape& aS1, const Shape& aS2)
		{
			return PlaneRay<T>(aS2, aS1);
		}

		template<typename T>
		inline CollisionResult<T> SphereAABB(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereAABB<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere),
				DownCastTo<AABB<T>>(aS2, Shape::Type::AABB));
		}
		template<typename T>
		inline CollisionResult<T> AABBSphere(const Shape& aS1, const Shape& aS2)
		{
			CollisionResult<T> result = SphereAABB<T>(aS2, aS1);
			result.normal = -result.normal; // flip normal ??

			return result;
		}

		template<typename T>
		inline CollisionResult<T> AABBRay(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionAABBRay<T>(
				DownCastTo<AABB<T>>(aS1, Shape::Type::AABB),
				DownCastTo<Ray<T>>(aS2, Shape::Type::Ray));
		}
		template<typename T>
		inline CollisionResult<T> RayAABB(const Shape& aS1, const Shape& aS2)
		{
			return AABBRay<T>(aS2, aS1);
		}

		template<typename T>
		inline CollisionResult<T> AABBPlane(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionAABBPlane<T>(
				DownCastTo<AABB<T>>(aS1, Shape::Type::AABB),
				DownCastTo<Plane<T>>(aS2, Shape::Type::Plane));
		}
		template<typename T>
		inline CollisionResult<T> PlaneAABB(const Shape& aS1, const Shape& aS2)
		{
			return AABBPlane<T>(aS2, aS1);
		}

		template<typename T>
		inline CollisionResult<T> AABBCapsule(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionAABBCapsule<T>(
				DownCastTo<AABB<T>>(aS1, Shape::Type::AABB),
				DownCastTo<Capsule<T>>(aS2, Shape::Type::Capsule));
		}
		template<typename T>
		inline CollisionResult<T> CapsuleAABB(const Shape& aS1, const Shape& aS2)
		{
			return AABBCapsule<T>(aS2, aS1);
		}

		template<typename T>
		inline CollisionResult<T> SphereRay(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereRay<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere),
				DownCastTo<Ray<T>>(aS2, Shape::Type::Ray));
		}
		template<typename T>
		inline CollisionResult<T> RaySphere(const Shape& aS1, const Shape& aS2)
		{
			return SphereRay<T>(aS2, aS1);
		}

		template<typename T>
		inline CollisionResult<T> SpherePlane(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSpherePlane<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere),
				DownCastTo<Plane<T>>(aS2, Shape::Type::Plane));
		}
		template<typename T>
		inline CollisionResult<T> PlaneSphere(const Shape& aS1, const Shape& aS2)
		{
			return SpherePlane<T>(aS2, aS1);
		}

		template<typename T>
		inline CollisionResult<T> SphereCapsule(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereCapsule<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere),
				DownCastTo<Capsule<T>>(aS2, Shape::Type::Capsule));
		}
		template<typename T>
		inline CollisionResult<T> CapsuleSphere(const Shape& aS1, const Shape& aS2)
		{
			return SphereCapsule<T>(aS2, aS1);
		}

		template<typename T>
		inline CollisionResult<T> CapsulePlane(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereCapsule<T>(
				DownCastTo<Capsule<T>>(aS1, Shape::Type::Capsule),
				DownCastTo<Plane<T>>(aS2, Shape::Type::Plane));
		}
		template<typename T>
		inline CollisionResult<T> PlaneCapsule(const Shape& aS1, const Shape& aS2)
		{
			return CapsulePlane<T>(aS2, aS1);
		}

		///		ab	sp	li	lv	pl	pv	ry  cu
		/// ab |-X-|-X-|---|---|---|---|-X-|-X-|
		/// sp |-X-|-X-|---|---|---|---|-X-|-X-|
		/// li |---|---|---|---|---|---|---|---|
		/// lv |---|---|---|---|---|---|---|---|
		/// pl |---|---|---|---|---|---|-X-|-X-|
		/// pv |---|---|---|---|---|---|---|---|
		/// ry |-X-|-X-|---|---|-X-|---|---|---|
		/// cu |-X-|-X-|---|---|-X-|---|---|-X-|
		/// 
		template<typename T>
		inline static std::array<CollisionResult<T>(*)(const Shape&, const Shape&),
			static_cast<int>(Shape::Type::Count) * static_cast<int>(Shape::Type::Count)> globalCollisionMatrix
		{
			// # aabb			# sphere			# line				# line volume		# plane				# plane volume		# ray				#capsule
			AABBAABB<T>,		AABBSphere<T>,		nullptr,			nullptr,			AABBPlane<T>,		nullptr,			AABBRay<T>,			AABBCapsule<T>,
			SphereAABB<T>,		SphereSphere<T>,	nullptr,			nullptr,			SpherePlane<T>,		nullptr,			SphereRay<T>,		SphereCapsule<T>,
			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,		
			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,
			PlaneAABB<T>,		PlaneSphere<T>,		nullptr,			nullptr,			nullptr,			nullptr,			PlaneRay<T>,		PlaneCapsule<T>,
			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,
			RayAABB<T>,			RaySphere<T>,		nullptr,			nullptr,			RayPlane<T>,		nullptr,			nullptr,			nullptr,
			CapsuleAABB<T>,		CapsuleSphere<T>,	nullptr,			nullptr,			CapsulePlane<T>,	nullptr,			nullptr,			CapsuleCapsule<T>
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
	inline CollisionResult<T> Collide(const Shape& aFirstShape, const Shape& aSecondShape)
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
	inline CollisionResult<T> IntersectionSphereSphere(const Sphere<T>& aFirstSphere, const Sphere<T>& aSecondSphere)
	{
		CollisionResult<T> result{};

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
		result.collided		= true;

		return result;
	}

	template<typename T>
	CollisionResult<T> IntersectionAABBAABB(const AABB<T>& aFirstAABB, const AABB<T>& aSecondAABB)
	{
		CollisionResult<T> result{};

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
						result.collided		= true;
					}
					else if (yOverlap < xOverlap && yOverlap < zOverlap) // y-axis
					{
						result.normal		= (dir.y < 0) ? Vector3<T>(0, 1, 0) : Vector3<T>(0, -1, 0);
						result.penetration	= yOverlap;
						result.collided		= true;
					}
					else // z-axis
					{
						result.normal		= (dir.z < 0) ? Vector3<T>(0, 0, 1) : Vector3<T>(0, 0, -1);
						result.penetration	= zOverlap;
						result.collided		= true;
					}
				}
				//else // 2D collision
				//{
				//	if (xOverlap < yOverlap) // x-axis
				//	{
				//		result.normal = (dir.x < 0) ? Vector3<T>(1, 0, 0) : Vector3<T>(-1, 0, 0);
				//		result.penetration = xOverlap;
				//		result.collided = true;
				//	}
				//	else  // y-axis
				//	{
				//		result.normal = (dir.y < 0) ? Vector3<T>(0, 1, 0) : Vector3<T>(0, -1, 0);
				//		result.penetration = yOverlap;
				//		result.collided = true;
				//	}
				//}
			}
		}

		return result;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionCapsuleCapsule(const Capsule<T>& aFirstCapsule, const Capsule<T>& aSecondCapsule)
	{
		auto [p1, p2] = Vector3<T>::ClosestPointsSegmentSegment(aFirstCapsule.GetTip(), aFirstCapsule.GetBase(), aSecondCapsule.GetTip(), aSecondCapsule.GetBase());
		return IntersectionSphereSphere(Sphere<T>(p1, aFirstCapsule.GetRadius()), Sphere<T>(p2, aSecondCapsule.GetRadius()));
	}

	template<typename T>
	inline CollisionResult<T> IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay)
	{
		CollisionResult<T> result{};

		T numen = Vector3<T>::Direction(aRay.GetOrigin(), aPlane.GetOrigin()).Dot(aPlane.GetNormal());
		T denom = aPlane.GetNormal().Dot(aRay.GetDirection());

		if (Equal<T>(denom, 0))
		{
			// if the numenator is zero then the origin of the ray lies on plane, which it must if parallel to the plane and cause intersection
			if (numen == 0)
			{
				result.intersection = aRay.GetOrigin();
				result.normal		= aPlane.GetNormal();
				result.collided		= true;
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
		result.collided		= true;

		return result;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionSphereAABB(const Sphere<T>& aSphere, const AABB<T>& aAABB)
	{
		CollisionResult<T> result{};

		const auto Clamp = [](T aValue, T aMin, T aMax) { return (aValue < aMin) ? aMin : ((aValue > aMax) ? aMax : aValue); };

		Vector3<T> aabbCenter = aAABB.GetCenter();
		Vector3<T> dir = Vector3<T>::Direction(aabbCenter, aSphere.GetCenter());

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
		result.collided		= true;

		return result;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionAABBRay(const AABB<T>& aAABB, const Ray<T>& aRay)
	{
		CollisionResult<T> result{};

		const Vector3<T> t1 = (aAABB.GetMin() - aRay.GetOrigin()) / aRay.GetDirection();
		const Vector3<T> t2 = (aAABB.GetMax() - aRay.GetOrigin()) / aRay.GetDirection();

		T minT = 0;
		T maxT = MAX_V<T>;

		minT = Min(Max(t1.x, minT), Max(t2.x, minT));
		maxT = Max(Min(t1.x, maxT), Min(t2.x, maxT));

		minT = Min(Max(t1.y, minT), Max(t2.y, minT));
		maxT = Max(Min(t1.y, maxT), Min(t2.y, maxT));

		minT = Min(Max(t1.z, minT), Max(t2.z, minT));
		maxT = Max(Min(t1.z, maxT), Min(t2.z, maxT));

		if (minT > maxT)
			return result;

		minT = Max(minT, T(0)); // clamp t to zero if inside aabb

		result.intersection = aRay.GetOrigin() + aRay.GetDirection() * minT;
		result.enter		= minT;
		result.exit			= maxT;
		result.collided		= true;

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

		return result;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionAABBPlane(const AABB<T>& aAABB, const Plane<T>& aPlane)
	{
		return CollisionResult<T>();
	}

	template<typename T>
	inline CollisionResult<T> IntersectionAABBCapsule(const AABB<T>& aAABB, const Capsule<T>& aCapsule)
	{
		return CollisionResult<T>();
	}

	template<typename T>
	inline CollisionResult<T> IntersectionSphereRay(const Sphere<T>& aSphere, const Ray<T>& aRay)
	{
		CollisionResult<T> result{};

		Vector3<T> dir = Vector3<T>::Direction(aRay.GetOrigin(), aSphere.GetCenter());

		T distSqr = dir.LengthSqr() - aSphere.GetRadiusSqr();
		T projScalar = dir.Dot(aRay.GetDirection());

		if (distSqr > 0 && projScalar < 0) // outside sphere and pointing away
		{
			return result;
		}

		T discr = projScalar * projScalar - distSqr;

		if (discr < 0) // closest point is outside
		{
			return result;
		}

		discr = std::sqrt(discr);

		T t1 = Max(projScalar - discr, T(0)); // clamp t to zero if inside sphere
		T t2 = projScalar + discr;

		result.intersection = aRay.GetOrigin() + aRay.GetDirection() * t1;
		result.normal		= Vector3<T>::Direction(aSphere.GetCenter(), result.intersection).GetNormalized();
		result.enter		= t1;
		result.exit			= t2;
		result.collided		= true;

		return result;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionSpherePlane(const Sphere<T>& aSphere, const Plane<T>& aPlane)
	{
		return CollisionResult<T>();
	}

	template<typename T>
	inline CollisionResult<T> IntersectionSphereCapsule(const Sphere<T>& aSphere, const Capsule<T>& aCapsule)
	{
		Vector3<T> p = Vector3<T>::ClosestPointOnSegment(aCapsule.GetTip(), aCapsule.GetBase(), aSphere.GetCenter());
		return IntersectionSphereSphere(aSphere, Sphere<T>(p, aCapsule.GetRadius()));
	}

	template<typename T>
	inline CollisionResult<T> IntersectionCapsulePlane(const Capsule<T>& aCapsule, const Plane<T>& aPlane)
	{
		return CollisionResult<T>();
	}

	using CollResFloat	= CollisionResult<float>;
	using CollResInt	= CollisionResult<int>;
	using CollResDouble = CollisionResult<double>;
}