#include <iostream>
#include <string>
#include <array>

#include <CommonUtilities/Math/Intersection.hpp>
#include <CommonUtilities/Math/Matrix4x4.hpp>
#include <CommonUtilities/Utility/Benchmark.h>
#include <CommonUtilities\Utility\Win32Utils.h>
#include <CommonUtilities/Serialization/BinarySerializer.h>
#include <CommonUtilities/Utility/TypeUtils.hpp>
#include <CommonUtilities/Utility/Easings.hpp>

#include <CommonUtilities/Event/TimedEvent.h>

#include <CommonUtilities/Structures/Blackboard.hpp>

#include <CommonUtilities/Utility/BitUtils.hpp>
#include <CommonUtilities/Math/AABB.hpp>

#include <CommonUtilities/Serialization/BinarySerializer.h>

#include <CommonUtilities/Thread/Parallell.hpp>

int main()
{
	cu::Vector2f vec(5.0f, 3.0f);
	auto l = vec.ToSIMD();

	cu::Mat4f mat1 = cu::Mat4f::CreateTRS(cu::Vector3f(0.0f, 5.0f, 0.0f), cu::Vector3f(0.5f, 0.2f, -2.5f), cu::Vector3f::One);
	cu::Mat4f mat2 = cu::Mat4f::CreateTRS(cu::Vector3f(0.0f, 5.0f, 0.0f), cu::Vector3f(0.2f, 0.9f, -0.5f), cu::Vector3f::One);

	cu::Mat4f r1 = mat1 * mat2;
	cu::Mat4f r2 = cu::Mat4f::Multiply(mat1, mat2);

	cu::AABBf aabb;
	cu::Rayf ray;

	constexpr cu::Capsulef caps = cu::Capsulef(cu::Vector3f(0.0f, 0.0f, 0.0f), cu::Vector3f(0.0f, 10.0f, 0.0f), 16.0f);

	const cu::ISectf intersection = cu::IntersectionCapsuleSegment(
		caps,
		cu::Vector3f(-32.0f, 1.0f, 2.0f), 
		cu::Vector3f(32.0f, 1.0f, 2.0f));

	constexpr cu::AABBf test = caps.GetAABB();

	//cu::BinaryWriteSerializer write;
	//std::tuple<std::string, int, float> abc("hello", 5, 9.0f);
	//write.Serialize(abc);
	
	constexpr std::uint64_t packed = cu::PackValues64<8, 4, 9, 43>({ 3, 5, 1, 3 });
	constexpr std::uint64_t extracted = cu::ExtractValue64<43, 21>(packed);
	std::string bits = cu::ToBinary(packed);
	std::string bits2 = cu::ToBinary(extracted);

	std::wstring testa = L"aoiasdfé 22";
	std::wstring testa2;

	cu::BinaryWriteSerializer write;
	write << testa;

	std::vector<int> aBc;

	cu::ParallelFor(write.GetBuffer(),
		[](std::byte aLeft)
		{
			
		});

	cu::BinaryReadSerializer read(write.GetBuffer());
	read >> testa2;

	return 0;
}