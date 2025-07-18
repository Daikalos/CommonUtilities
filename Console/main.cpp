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

#include <CommonUtilities/Thread/Parallel.hpp>
#include <CommonUtilities/Alloc/ArenaAlloc.hpp>

#include <CommonUtilities/Random/Random.hpp>

#include <CommonUtilities/System/Color.hpp>
#include <CommonUtilities/System/LinearCurve.hpp>

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

	//std::vector<int, cu::ArenaAlloc<int>> wow;
	//for (int i = 0; i < 100000; ++i)
	//	wow.emplace_back(cu::Random(-10000000, 10000000));

	//for (int a : wow)
	//	std::cout << a << '\n';

	constexpr cu::Vector4f colVec = cu::Vector4f(3.0f, 0.99f, 0.5f, -54.0f);
	constexpr cu::Vector4f testrVec = cu::Vector4f::CLerp(cu::Vector4f(0.5f, 0.2f, 0.4f, 1.0f), cu::Vector4f(-0.2f, 0.5f, 10.0f, 1.0f), 0.5f);

	cu::Color color = cu::Color(126, 32, 45, 200);
	cu::Color color2 = cu::Color::CLerp(color, cu::Color(190, 90, 200, 50), -2.5f);

	cu::ColorGradient gradient;
	gradient.Reset(cu::Color::Black, cu::Color::White);
	cu::Color color3 = gradient.Get(0.5f);

	cu::FreeVector<int> testabc;
	testabc.emplace(59);

	return 0;
}