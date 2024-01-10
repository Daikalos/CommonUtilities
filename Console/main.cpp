#include <iostream>
#include <memory>

#include <CommonUtilities/Utility/Random.hpp>
#include <CommonUtilities/Config.h>

#include <CommonUtilities/Utility/ArithmeticUtils.hpp>
#include <CommonUtilities/Math/Vector.hpp>
#include <CommonUtilities/Math/Transform2D.h>
#include <CommonUtilities/Math/Relation2D.h>
#include <CommonUtilities/Math/Vector4.hpp>
#include <CommonUtilities/System/Event.hpp>
#include <CommonUtilities/Input/ButtonEvent.hpp>
#include <CommonUtilities/Input/KeyboardInput.h>
#include <CommonUtilities/System/Color.hpp>
#include <CommonUtilities/Math/Sphere.hpp>

#include <CommonUtilities/System/StateStack.hpp>
#include <CommonUtilities/System/StateMachine.hpp>

class TestState : public cu::StateStack<std::string>::State
{
public:
	using cu::StateStack<std::string>::State::State;

	bool Update(cu::Timer& aTimer) override
	{
		return false;
	}
};

class TestState2 : public cu::StateMachine<std::string>::State
{
public:
	using cu::StateMachine<std::string>::State::State;

	void Enter() override
	{
		std::cout << "enter";
	}

	void Update(cu::Timer& aTimer) override
	{
	}

	void Exit() override
	{
	}

};

int main()
{
	cu::Vector2f test1(cu::Random(-5000.0f, 5000.0f), 2423.5453253f);
	cu::Vector2f test2(cu::Random(-5000.0f, 5000.0f), 21331.423025f);

	test1.Normalize(1.0f);
	test2.Normalize();

	std::cout << test1.x << test2.x;

	cu::StateStack<std::string> stateStack;
	stateStack.RegisterState<TestState>("hello");
	stateStack.Push("hello");
	stateStack.ApplyPendingChanges();

	cu::StateMachine<std::string> stateMachine;
	stateMachine.AddState<TestState2>("hey");
	stateMachine.TransitionTo("hey");

	cu::Relation2DPtr relation1 = cu::Relation2D::Instantiate();
	cu::Relation2DPtr relation2 = cu::Relation2D::Instantiate();
	cu::Relation2DPtr relation3 = cu::Relation2D::Instantiate();

	cu::Relation2D::Attach(relation2, relation1);
	relation2->Move(cu::Vector2f(5.0f, 5.0f));

	const cu::Mat3f& globalMatrix = relation1->GetGlobalMatrix();

	constexpr cu::Sphere<float> sphere(cu::Vector3f(5.0f, 2.0f, -7.0f), 2.0f);
	constexpr cu::Shape::Type type = sphere.GetType();

	return 0;
}