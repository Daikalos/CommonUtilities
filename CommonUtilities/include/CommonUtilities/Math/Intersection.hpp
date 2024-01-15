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
#include "AABB3D.hpp"
#include "Vector3.hpp"

namespace CommonUtilities
{
	template<typename T>
	struct CollisionResult 
	{
		Vector3<T>	intersection;
		Vector3<T>	normal			{1.0f, 0.0f, 0.0f};	// default normal points right
		float		penetration		{0.0f};
		bool		collided		{false};

		operator bool() const noexcept // implicitly convertable to boolean
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
				DownCastTo<AABB3D<T>>(aS1, Shape::Type::AABB3D), 
				DownCastTo<AABB3D<T>>(aS2, Shape::Type::AABB3D));
		}

		template<typename T>
		inline CollisionResult<T> SphereSphere(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionSphereSphere<T>(
				DownCastTo<Sphere<T>>(aS1, Shape::Type::Sphere), 
				DownCastTo<Sphere<T>>(aS2, Shape::Type::Sphere));
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
				DownCastTo<AABB3D<T>>(aS2, Shape::Type::AABB3D));
		}
		template<typename T>
		inline CollisionResult<T> AABBSphere(const Shape& aS1, const Shape& aS2)
		{
			CollisionResult<T> result = SphereAABB<T>(aS2, aS1);
			//result.normal = -result.normal; // flip normal ??

			return result;
		}

		template<typename T>
		inline CollisionResult<T> AABBRay(const Shape& aS1, const Shape& aS2)
		{
			return IntersectionAABBRay<T>(
				DownCastTo<AABB3D<T>>(aS1, Shape::Type::AABB3D),
				DownCastTo<Ray<T>>(aS2, Shape::Type::Ray));
		}
		template<typename T>
		inline CollisionResult<T> RayAABB(const Shape& aS1, const Shape& aS2)
		{
			return AABBRay<T>(aS2, aS1);
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

		///		ab	sp	li	lv	pl	pv	ry 
		/// ab |-X-|-X-|---|---|---|---|-X-|
		/// sp |-X-|-X-|---|---|---|---|-X-|
		/// li |---|---|---|---|---|---|---|
		/// lv |---|---|---|---|---|---|---|
		/// pl |---|---|---|---|---|---|-X-|
		/// pv |---|---|---|---|---|---|---|
		/// ry |-X-|-X-|---|---|-X-|---|---|
		/// 
		template<typename T>
		inline static std::array<CollisionResult<T>(*)(const Shape&, const Shape&),
			static_cast<int>(Shape::Type::Count) * static_cast<int>(Shape::Type::Count)> globalCollisionMatrix
		{
			// # aabb			# sphere			# line				# line volume		# plane				# plane volume		# ray
			AABBAABB<T>,		AABBSphere<T>,		nullptr,			nullptr,			nullptr,			nullptr,			AABBRay<T>,
			SphereAABB<T>,		SphereSphere<T>,	nullptr,			nullptr,			nullptr,			nullptr,			SphereRay<T>,
			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,
			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,
			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			PlaneRay<T>,
			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,			nullptr,
			RayAABB<T>,			RaySphere<T>,		nullptr,			nullptr,			RayPlane<T>,		nullptr,			nullptr
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
			throw std::out_of_range("No collision algorithm is defined for these two shapes!");
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

		if (distSqr > FLT_EPSILON * FLT_EPSILON)
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
	CollisionResult<T> IntersectionAABBAABB(const AABB3D<T>& aFirstAABB, const AABB3D<T>& aSecondAABB)
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
				else // 2D collision
				{
					if (xOverlap < yOverlap) // x-axis
					{
						result.normal = (dir.x < 0) ? Vector3<T>(1, 0, 0) : Vector3<T>(-1, 0, 0);
						result.penetration = xOverlap;
						result.collided = true;
					}
					else  // y-axis
					{
						result.normal = (dir.y < 0) ? Vector3<T>(0, 1, 0) : Vector3<T>(0, -1, 0);
						result.penetration = yOverlap;
						result.collided = true;
					}
				}
			}
		}

		return result;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionPlaneRay(const Plane<T>& aPlane, const Ray<T>& aRay)
	{
		CollisionResult<T> result{};

		T numen = Vector3<T>::Direction(aRay.GetOrigin(), aPlane.GetOrigin()).Dot(aPlane.GetNormal());
		T denom = aPlane.GetNormal().Dot(aRay.GetDirection());

		if (static_cast<T>(std::abs(denom)) <= static_cast<T>(FLT_EPSILON))
		{
			// if the numenator is zero then the origin of the ray lies on plane, which it must if parallel to the plane and allow intersection
			if (numen == 0)
			{
				result.intersection = aRay.GetOrigin();
				result.normal		= aPlane.GetNormal();
				result.penetration	= 0.0f;
				result.collided		= true;

				return result;
			}

			return result;
		}

		T t = numen / denom;

		if (t < 0) // ray points away from plane
		{
			return result;
		}

		result.intersection = aRay.GetOrigin() + aRay.GetDirection() * t;
		result.normal		= aPlane.GetNormal() * -au::Sign<T>(denom); // flip normal based on what side we are approaching from
		result.penetration	= 0.0f; 
		result.collided		= true;

		return result;
	}

	template<typename T>
	inline CollisionResult<T> IntersectionSphereAABB(const Sphere<T>& aSphere, const AABB3D<T>& aAABB3D)
	{
		CollisionResult<T> result{};

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

		if (distance > FLT_EPSILON * FLT_EPSILON)
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
	inline CollisionResult<T> IntersectionAABBRay(const AABB3D<T>& aAABB3D, const Ray<T>& aRay)
	{
		CollisionResult<T> result{};

		if (aAABB3D.IsInside(aRay.GetOrigin()))
		{
			const T x = static_cast<T>(std::abs(aRay.GetDirection().x));
			const T y = static_cast<T>(std::abs(aRay.GetDirection().y));
			const T z = static_cast<T>(std::abs(aRay.GetDirection().z));

			if (x > y && x > z)
			{
				result.normal = Vector3<T>(-au::Sign<T>(aRay.GetDirection().x), 0, 0);
			}
			else if (y > x && y > z)
			{
				result.normal = Vector3<T>(0, -au::Sign<T>(aRay.GetDirection().y), 0);
			}
			else
			{
				result.normal = Vector3<T>(0, 0, -au::Sign<T>(aRay.GetDirection().z));
			}


			result.intersection = aRay.GetOrigin();
			result.penetration	= 0.0f;
			result.collided		= true;

			return result;
		}

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

				result.normal = Vector3<T>(au::Sign<T>(aRay.GetDirection().x), 0, 0);

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

				result.normal = Vector3<T>(0, au::Sign<T>(aRay.GetDirection().y), 0);

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

				result.normal = Vector3<T>(0, 0, au::Sign<T>(aRay.GetDirection().z));

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
		CollisionResult<T> result{};

		Vector3<T> dir = Vector3<T>::Direction(aSphere.GetCenter(), aRay.GetOrigin());

		T distSqr = dir.LengthSqr() - aSphere.GetRadiusSqr();
		T projScalar = dir.Dot(aRay.GetDirection());

		if (distSqr > 0 && projScalar > 0) // outside sphere and pointing away
		{
			return result;
		}

		T discr = projScalar * projScalar - distSqr;

		if (discr < 0) // closest point is outside
		{
			return result;
		}

		T t = -projScalar - std::sqrt(discr);

		if (t < 0) t = 0; // clamp t to zero if inside sphere

		result.intersection = aRay.GetOrigin() + aRay.GetDirection() * t;
		result.normal		= Vector3<T>::Direction(aSphere.GetCenter(), result.intersection).GetNormalized();
		result.penetration	= 0.0f;
		result.collided		= true;

		return result;
	}

	using CollResFloat	= CollisionResult<float>;
	using CollResInt	= CollisionResult<int>;
	using CollResDouble = CollisionResult<double>;
}