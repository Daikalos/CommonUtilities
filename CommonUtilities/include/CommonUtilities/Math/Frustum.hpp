#pragma once

#include <array>
#include <span>
#include <numeric>

#include <CommonUtilities/Structures/EnumArray.hpp>

#include "Matrix4x4.hpp"
#include "Plane.hpp"
#include "Sphere.hpp"
#include "AABB.hpp"

namespace CommonUtilities
{
	template<typename T>
	class Frustum
	{
	public:
		enum class Face
		{
			Right,
			Left,
			Top,
			Bottom,
			Far,
			Near
		};

		using FrustumPlanes = EnumArray<Face, Plane<T>, 6>;
		using FrustumPoints = std::array<Vector3<T>, 8>;

		constexpr Frustum() = default;
		constexpr ~Frustum() = default;

		constexpr Frustum(const Matrix4x4<T>& aViewProjection);
		constexpr Frustum(const AABB<T>& aBox);
		constexpr Frustum(const FrustumPoints& aPoints);

		NODISC constexpr const FrustumPlanes& GetPlanes() const noexcept;
		NODISC constexpr const FrustumPoints& GetPoints() const noexcept;
		NODISC constexpr const Vector3<T>& GetCenter() const noexcept;
		NODISC constexpr const Vector3<T>& GetNearCenter() const noexcept;
		NODISC constexpr const Vector3<T>& GetFarCenter() const noexcept;

		constexpr Frustum& SetPoints(const Matrix4x4<T>& aViewProjection);
		constexpr Frustum& SetPoints(const AABB<T>& aBox);
		constexpr Frustum& SetPoints(const FrustumPoints& aPoints);

		template<std::size_t N>
		NODISC constexpr std::array<Frustum, N> Subdivide(std::span<const float, N> aSubdivisions) const;

		NODISC constexpr Frustum GetSubFrustum(float aStartPercentage, float aEndPercentage) const;

		NODISC constexpr bool IsInside(const Sphere<T>& aSphere) const;
		NODISC constexpr bool IsInside(const AABB<T>& aBox) const;

		NODISC constexpr bool IsInsideNoDepth(const Sphere<T>& aSphere) const;
		NODISC constexpr bool IsInsideNoDepth(const AABB<T>& aBox) const;

	private:
		constexpr void ComputeCenters();
		constexpr void ComputePlanes();

		FrustumPlanes	myPlanes;
		FrustumPoints	myPoints;
		Vector3<T>		myCenter;
		Vector3<T>		myNearCenter;
		Vector3<T>		myFarCenter;
	};

	template<typename T>
	constexpr Frustum<T>::Frustum(const Matrix4x4<T>& aViewProjection)
	{
		SetPoints(aViewProjection);
	}
	template<typename T>
	constexpr Frustum<T>::Frustum(const AABB<T>& aBox)
	{
		SetPoints(aBox);
	}
	template<typename T>
	constexpr Frustum<T>::Frustum(const FrustumPoints& aPoints)
	{
		SetPoints(aPoints);
	}

	template<typename T>
	constexpr const Frustum<T>::FrustumPlanes& Frustum<T>::GetPlanes() const noexcept
	{
		return myPlanes;
	}
	template<typename T>
	constexpr const Frustum<T>::FrustumPoints& Frustum<T>::GetPoints() const noexcept
	{
		return myPoints;
	}
	template<typename T>
	constexpr const Vector3<T>& Frustum<T>::GetCenter() const noexcept
	{
		return myCenter;
	}
	template<typename T>
	constexpr const Vector3<T>& Frustum<T>::GetNearCenter() const noexcept
	{
		return myNearCenter;
	}
	template<typename T>
	constexpr const Vector3<T>& Frustum<T>::GetFarCenter() const noexcept
	{
		return myFarCenter;
	}

	template<typename T>
	constexpr Frustum<T>& Frustum<T>::SetPoints(const Matrix4x4<T>& aViewProjection)
	{
		const cu::Mat4f invMat = aViewProjection.GetInverse();

		const Vector4<T> p0 = invMat * Vector4<T>(-1.0f,  1.0f, 0.0f, 1.0f);
		const Vector4<T> p1 = invMat * Vector4<T>( 1.0f,  1.0f, 0.0f, 1.0f);
		const Vector4<T> p2 = invMat * Vector4<T>(-1.0f, -1.0f, 0.0f, 1.0f);
		const Vector4<T> p3 = invMat * Vector4<T>( 1.0f, -1.0f, 0.0f, 1.0f);
		const Vector4<T> p4 = invMat * Vector4<T>(-1.0f,  1.0f, 1.0f, 1.0f);
		const Vector4<T> p5 = invMat * Vector4<T>( 1.0f,  1.0f, 1.0f, 1.0f);
		const Vector4<T> p6 = invMat * Vector4<T>(-1.0f, -1.0f, 1.0f, 1.0f);
		const Vector4<T> p7 = invMat * Vector4<T>( 1.0f, -1.0f, 1.0f, 1.0f);
			   
		const Vector3<T> nearTopLeft	= p0.XYZ() / p0.w;
		const Vector3<T> nearTopRight	= p1.XYZ() / p1.w;
		const Vector3<T> nearBotLeft	= p2.XYZ() / p2.w;
		const Vector3<T> nearBotRight	= p3.XYZ() / p3.w;
		const Vector3<T> farTopLeft		= p4.XYZ() / p4.w;
		const Vector3<T> farTopRight	= p5.XYZ() / p5.w;
		const Vector3<T> farBotLeft		= p6.XYZ() / p6.w;
		const Vector3<T> farBotRight	= p7.XYZ() / p7.w;

		myPoints[0] = nearTopLeft;
		myPoints[1] = nearTopRight;
		myPoints[2] = nearBotLeft;
		myPoints[3] = nearBotRight;

		myPoints[4] = farTopLeft;
		myPoints[5] = farTopRight;
		myPoints[6] = farBotLeft;
		myPoints[7] = farBotRight;

		ComputeCenters();
 		ComputePlanes();

		return *this;
	}
	template<typename T>
	constexpr Frustum<T>& Frustum<T>::SetPoints(const AABB<T>& aBox)
	{
		return SetPoints(aBox.GetPoints());
	}
	template<typename T>
	constexpr Frustum<T>& Frustum<T>::SetPoints(const FrustumPoints& aPoints)
	{
		myPoints = aPoints;

		ComputeCenters();
 		ComputePlanes();

		return *this;
	}

	template<typename T>
	constexpr Frustum<T> Frustum<T>::GetSubFrustum(float aStartPercentage, float aEndPercentage) const
	{
		Frustum result;

		const Vector3<T> nearTopLeft	= Vector3<T>::CLerp(myPoints[0], myPoints[4], (T)aStartPercentage);
		const Vector3<T> nearTopRight	= Vector3<T>::CLerp(myPoints[1], myPoints[5], (T)aStartPercentage);
		const Vector3<T> nearBotLeft	= Vector3<T>::CLerp(myPoints[2], myPoints[6], (T)aStartPercentage);
		const Vector3<T> nearBotRight	= Vector3<T>::CLerp(myPoints[3], myPoints[7], (T)aStartPercentage);
		 										 
		const Vector3<T> farTopLeft		= Vector3<T>::CLerp(myPoints[0], myPoints[4], (T)aEndPercentage);
		const Vector3<T> farTopRight	= Vector3<T>::CLerp(myPoints[1], myPoints[5], (T)aEndPercentage);
		const Vector3<T> farBotLeft		= Vector3<T>::CLerp(myPoints[2], myPoints[6], (T)aEndPercentage);
		const Vector3<T> farBotRight	= Vector3<T>::CLerp(myPoints[3], myPoints[7], (T)aEndPercentage);

		result.myPoints[0] = nearTopLeft;
		result.myPoints[1] = nearTopRight;
		result.myPoints[2] = nearBotLeft;
		result.myPoints[3] = nearBotRight;
						 
		result.myPoints[4] = farTopLeft;
		result.myPoints[5] = farTopRight;
		result.myPoints[6] = farBotLeft;
		result.myPoints[7] = farBotRight;

		result.ComputeCenters();
		result.ComputePlanes();

		return result;
	}

	template<typename T>
	constexpr bool Frustum<T>::IsInside(const Sphere<T>& aSphere) const
	{
		float radius = -aSphere.GetRadius();

		return 
			!(myPlanes[Face::Right].IsInside(aSphere.GetCenter(), radius) ||
			  myPlanes[Face::Left].IsInside(aSphere.GetCenter(), radius) ||
			  myPlanes[Face::Top].IsInside(aSphere.GetCenter(), radius) ||
			  myPlanes[Face::Bottom].IsInside(aSphere.GetCenter(), radius) ||
			  myPlanes[Face::Far].IsInside(aSphere.GetCenter(), radius) ||
			  myPlanes[Face::Near].IsInside(aSphere.GetCenter(), radius));
	}
	template<typename T>
	constexpr bool Frustum<T>::IsInside(const AABB<T>& aBox) const
	{
		for (int i = 0; i < 6; i++)
		{
			Face f = static_cast<Face>(i);

			const float d = myPlanes[f].GetDistance();

			int out = 0;
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMin().x, aBox.GetMin().y, aBox.GetMin().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMax().x, aBox.GetMin().y, aBox.GetMin().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMin().x, aBox.GetMax().y, aBox.GetMin().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMax().x, aBox.GetMax().y, aBox.GetMin().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMin().x, aBox.GetMin().y, aBox.GetMax().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMax().x, aBox.GetMin().y, aBox.GetMax().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMin().x, aBox.GetMax().y, aBox.GetMax().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMax().x, aBox.GetMax().y, aBox.GetMax().z)) + d) < T(0));

			if (out == 8) return false;
		}

		int out = 0;
		out = 0; for (int i = 0; i < 8; i++) out += (int)(myPoints[i].x > aBox.GetMax().x); if (out == 8) return false;
		out = 0; for (int i = 0; i < 8; i++) out += (int)(myPoints[i].x < aBox.GetMin().x); if (out == 8) return false;
		out = 0; for (int i = 0; i < 8; i++) out += (int)(myPoints[i].y > aBox.GetMax().y); if (out == 8) return false;
		out = 0; for (int i = 0; i < 8; i++) out += (int)(myPoints[i].y < aBox.GetMin().y); if (out == 8) return false;
		out = 0; for (int i = 0; i < 8; i++) out += (int)(myPoints[i].z > aBox.GetMax().z); if (out == 8) return false;
		out = 0; for (int i = 0; i < 8; i++) out += (int)(myPoints[i].z < aBox.GetMin().z); if (out == 8) return false;

		return true;

		//cu::Vector3f center = aBox.GetCenter();
		//cu::Vector3f extends = aBox.GetExtends();

		//return
		//	!(myPlanes[Face::Right].IsInside(center, extends) ||
		//	  myPlanes[Face::Left].IsInside(center, extends) ||
		//	  myPlanes[Face::Top].IsInside(center, extends) ||
		//	  myPlanes[Face::Bottom].IsInside(center, extends) ||
		//	  myPlanes[Face::Far].IsInside(center, extends) ||
		//	  myPlanes[Face::Near].IsInside(center, extends));
	}

	template<typename T>
	constexpr bool Frustum<T>::IsInsideNoDepth(const Sphere<T>& aSphere) const
	{
		const float radius = -aSphere.GetRadius();

		return 
			!(myPlanes[Face::Right].IsInside(aSphere.GetCenter(), radius) ||
			  myPlanes[Face::Left].IsInside(aSphere.GetCenter(), radius) ||
			  myPlanes[Face::Top].IsInside(aSphere.GetCenter(), radius) ||
			  myPlanes[Face::Bottom].IsInside(aSphere.GetCenter(), radius));
	}
	template<typename T>
	constexpr bool Frustum<T>::IsInsideNoDepth(const AABB<T>& aBox) const
	{
		for (int i = 0; i < 4; i++)
		{
			Face f = static_cast<Face>(i);

			const float d = myPlanes[f].GetDistance(); // plane distance

			int out = 0;
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMin().x, aBox.GetMin().y, aBox.GetMin().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMax().x, aBox.GetMin().y, aBox.GetMin().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMin().x, aBox.GetMax().y, aBox.GetMin().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMax().x, aBox.GetMax().y, aBox.GetMin().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMin().x, aBox.GetMin().y, aBox.GetMax().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMax().x, aBox.GetMin().y, aBox.GetMax().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMin().x, aBox.GetMax().y, aBox.GetMax().z)) + d) < T(0));
			out += (int)((myPlanes[f].GetNormal().Dot(Vector3<T>(aBox.GetMax().x, aBox.GetMax().y, aBox.GetMax().z)) + d) < T(0));

			if (out == 8) return false;
		}

		return true;
	}

	template<typename T>
	constexpr void Frustum<T>::ComputeCenters()
	{
		myCenter		= std::accumulate(myPoints.begin(), myPoints.end(), Vector3<T>()) / (T)myPoints.size();
		myNearCenter	= std::accumulate(myPoints.begin(), myPoints.begin() + myPoints.size() / 2, Vector3<T>()) / (T)(myPoints.size() / 2);
		myFarCenter		= std::accumulate(myPoints.begin() + myPoints.size() / 2, myPoints.end(), Vector3<T>()) / (T)(myPoints.size() / 2);
	}
	template<typename T>
	constexpr void Frustum<T>::ComputePlanes()
	{
		if (myPoints[0] == myPoints[1] || myPoints[0] == myPoints[2] || 
			myPoints[4] == myPoints[5] || myPoints[4] == myPoints[6])
			return;

		myPlanes[Face::Right]	= Plane<T>::InitWith3Points(myPoints[1], myPoints[7], myPoints[5]);
		myPlanes[Face::Left]	= Plane<T>::InitWith3Points(myPoints[0], myPoints[4], myPoints[6]);
		myPlanes[Face::Top]		= Plane<T>::InitWith3Points(myPoints[0], myPoints[5], myPoints[4]);
		myPlanes[Face::Bottom]	= Plane<T>::InitWith3Points(myPoints[2], myPoints[6], myPoints[7]);
		myPlanes[Face::Far]		= Plane<T>::InitWith3Points(myPoints[4], myPoints[5], myPoints[6]);
		myPlanes[Face::Near]	= Plane<T>::InitWith3Points(myPoints[2], myPoints[1], myPoints[0]);
	}

	template<typename T>
	template<std::size_t N>
	constexpr std::array<Frustum<T>, N> Frustum<T>::Subdivide(std::span<const float, N> aSubdivisions) const
	{
		std::array<Frustum, N> result;

		float startPercentage	= 0.0f;
		float endPercentage		= aSubdivisions[0];

		result[0] = GetSubFrustum(startPercentage, endPercentage);

		for (std::size_t i = 1; i < N; ++i)
		{
			startPercentage = aSubdivisions[i - 1];
			endPercentage	= aSubdivisions[i];

			result[i] = GetSubFrustum(startPercentage, endPercentage);
		}

		return result;
	}

	using Frustumf = Frustum<float>;
	using Frustumd = Frustum<double>;
	using Frustumi = Frustum<int>;
}