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
		NODISC const Vector2f& GetScreenSize() const noexcept;
		NODISC float GetNearClip() const noexcept;
		NODISC float GetFarClip() const noexcept;
		NODISC std::tuple<float, float> GetProjectionPlanes() const;

		NODISC Vector3f WorldToView(const Vector3f& aWorldPosition) const;
		NODISC Vector4f ViewToClip(const Vector3f& aViewPosition) const;
		NODISC Vector4f WorldToClip(const Vector3f& aWorldPosition) const;
		NODISC Vector3f ClipToNDC(const Vector4f& aClipPosition) const;
		NODISC Vector2f NDCToScreen(const Vector3f& aNDCPosition) const;
		NODISC Vector2f WorldToScreen(const Vector3f& aWorldPosition) const;

		virtual void SetOrtographicProjection(float aWidth, float aHeight, float aDepth);
		virtual void SetOrtographicProjection(float aLeft, float aRight, float aTop, float aBottom, float aNear, float aFar);
		virtual void SetPerspectiveProjection(float aHorizontalFOVDeg, const Vector2f& aResolution, float aNearClip, float aFarClip);

		virtual void SetTransform(const Transform3D& aTransform);
		virtual void SetPosition(const Vector3f& aPosition);
		virtual void SetRotation(const Vector3f& aRotation);

		virtual void Move(const Vector3f& aPosition);
		virtual void Rotate(const Vector3f& aRotation);

		virtual void SetScreenSize(const Vector2f& aScreenSize);

		virtual bool HandleEvent(UINT aMessage, WPARAM wParam, LPARAM lParam);

	private:
		Transform3D myTransform;
		Mat4f		myProjectionMatrix;
		Vector2f	myScreenSize;
		float		myFarClip	{1500.0f};
		float		myNearClip	{0.1f};
	};
}