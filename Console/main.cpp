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

#include <CommonUtilities/System/StateStack.hpp>
#include <CommonUtilities/System/StateMachine.hpp>

struct Foo
{
	int a;
};

class TestState : public cu::StateStack<Foo, std::string>::State
{
public:
	using cu::StateStack<Foo, std::string>::State::State;

	bool Update(cu::Timer& aTimer) override
	{
		return false;
	}
};

class TestState2 : public cu::StateMachine<Foo, std::string>::State
{
public:
	using cu::StateMachine<Foo, std::string>::State::State;

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

	cu::StateStack<Foo, std::string> stateStack(Foo(5));
	stateStack.RegisterState<TestState>("hello");
	stateStack.Push("hello");
	stateStack.ApplyPendingChanges();

	cu::StateMachine<Foo, std::string> stateMachine(Foo(6));
	stateMachine.AddState<TestState2>("hey");
	stateMachine.TransitionTo("hey");

	std::shared_ptr<cu::Relation2D> relation1 = std::make_shared<cu::Relation2D>();
	std::shared_ptr<cu::Relation2D> relation2 = std::make_shared<cu::Relation2D>();
	std::shared_ptr<cu::Relation2D> relation3 = std::make_shared<cu::Relation2D>();

	cu::Relation2D::Attach(relation2, relation1);
	relation2->Move(cu::Vector2f(5.0f, 5.0f));

	const cu::Mat3f& globalMatrix = relation1->GetGlobalMatrix();

	return 0;
}