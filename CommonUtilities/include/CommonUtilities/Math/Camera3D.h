#pragma once

#include <Windows.h>
#include <tuple>

#include <CommonUtilities/System/Timer.h>
#include <CommonUtilities/Config.h>

#include "Transform3D.h"
#include "Matrix4x4.hpp"
#include "Vector3.hpp"
#include "Vector2.hpp"

namespace CommonUtilities
{
	class COMMON_UTILITIES_API Camera3D
	{
	public:
		Camera3D() = default;
		virtual ~Camera3D() = default;

		NODISC const Transform3D& GetTransform() const noexcept;
		NODISC const Mat4f& GetProjection() const noexcept;
		NODISC const Vector3f& GetPosition() const noexcept;
		NODISC const Vector3f& GetRotation() const noexcept;
		NODISC const float GetNearClip() const noexcept;
		NODISC const float GetFarClip() const noexcept;
		NODISC std::tuple<float, float> GetProjectionPlanes() const;

		virtual void SetOrtographicProjection(float aWidth, float aHeight, float aDepth);
		virtual void SetOrtographicProjection(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar);
		virtual void SetPerspectiveProjection(float aHorizontalFoV, const Vector2f& aResolution, float aNearClip, float aFarClip);

		virtual void SetTransform(const Transform3D& aTransform);
		virtual void SetPosition(const Vector3f& aPosition);
		virtual void SetRotation(const Vector3f& aRotation);

		virtual void Move(const Vector3f& aPosition);
		virtual void Rotate(const Vector3f& aRotation);

	private:
		Transform3D myTransform;
		Mat4f		myProjectionMatrix;
		float		myFarClip	{1500.0f};
		float		myNearClip	{0.1f};
	};
}