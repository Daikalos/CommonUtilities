#include <iostream>

#include <CommonUtilities/Config.h>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>
#include <CommonUtilities/Math/Vector.hpp>
#include <CommonUtilities/Math/Transform2D.h>

#include <CommonUtilities/System/Event.hpp>
#include <CommonUtilities/Input/ButtonEvent.hpp>
#include <CommonUtilities/Input/KeyboardInput.h>

#include <CommonUtilities/Game/StateStack.hpp>

int main()
{
	cu::Mat3f mat = cu::Mat3f::CreateTRS(cu::Vector2f(100.0f, 100.0f), cu::PI_2, cu::Vector2f(1.0f, 1.0f));
	cu::Mat3f mat2;

	mat2.Combine(mat);

	cu::Mat3f inv = mat.FastInverse();
	float rotation = mat2.GetRotation() * cu::RAD2DEG<>;
	float rotation2 = mat.GetRotation() * cu::RAD2DEG<>;

	mat2.Combine(inv);

	cu::Vector4f vec1, vec2;

	(vec1 + vec2) = cu::Vector4f();

	cu::Event<int, const std::string&> test;

	test += [](int val, const std::string& text)
	{
		std::cout << val << " " << text << '\n';
	};

	test(5, "hello");

	cu::KeyboardInput keyboard;
	cu::ButtonEvent<cu::KeyboardInput> buttonEvent;
	buttonEvent.Add(cu::Keyboard::W, cu::BT_Pressed, []()
		{

		});

	return 0;
}