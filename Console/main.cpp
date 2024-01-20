#include <iostream>
#include <memory>

#include <CommonUtilities/Utility/Random.hpp>
#include <CommonUtilities/Config.h>

#include <CommonUtilities/Math/Intersection.hpp>
#include <CommonUtilities/Utility/ArithmeticUtils.hpp>
#include <CommonUtilities/Math/Vector.hpp>
#include <CommonUtilities/Math/Transform2D.h>
#include <CommonUtilities/Math/Relation2D.h>
#include <CommonUtilities/Math/Vector4.hpp>
#include <CommonUtilities/Math/AABB3D.hpp>
#include <CommonUtilities/System/Event.hpp>
#include <CommonUtilities/Input/ButtonEvent.hpp>
#include <CommonUtilities/Input/KeyboardInput.h>
#include <CommonUtilities/System/Color.hpp>
#include <CommonUtilities/Math/Sphere.hpp>
#include <CommonUtilities/Utility/Benchmark.h>
#include <CommonUtilities/Utility/PrintUtils.hpp>

#include <CommonUtilities/System/StateStack.hpp>
#include <CommonUtilities/System/StateMachine.hpp>
#include <CommonUtilities/Input/InputBind.hpp>

class MenuState : public cu::StateStack<int, std::string>::State
{
public:
	using cu::StateStack<int, std::string>::State::State;

private:

};

class MenuStack : public cu::StateStack<int, std::string>
{
public:
	using cu::StateStack<int, std::string>::StateStack;

	NODISC auto operator[](std::size_t aIndex) const -> const MenuState& override
	{
		return static_cast<const MenuState&>(cu::StateStack<int, std::string>::GetState(aIndex));
	}
	NODISC auto operator[](std::size_t aIndex) -> MenuState& override 
	{
		return static_cast<MenuState&>(cu::StateStack<int, std::string>::GetState(aIndex));
	}

	NODISC auto GetState(std::size_t aIndex) const -> const MenuState& override 
	{
		return static_cast<const MenuState&>(cu::StateStack<int, std::string>::GetState(aIndex));
	}
	NODISC auto GetState(std::size_t aIndex) -> MenuState& override 
	{
		return static_cast<MenuState&>(cu::StateStack<int, std::string>::GetState(aIndex));
	}

	NODISC auto GetStateByID(const std::string& aStateID) const -> const MenuState* override 
	{
		return static_cast<const MenuState*>(cu::StateStack<int, std::string>::GetStateByID(aStateID));
	}
	NODISC auto GetStateByID(const std::string& aStateID) -> MenuState* override 
	{ 
		return static_cast<MenuState*>(cu::StateStack<int, std::string>::GetStateByID(aStateID));
	}

private:

};

class TestState : public MenuState
{
public:
	using MenuState::MenuState;

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
	enum class GameButton
	{
		None = -1,
		Up,
		Left,
		Right,
		Down,
	};

	cu::bm::Begin();
	cu::AABB3D<float> aabb = cu::AABB3D<float>::InitWithCenterAndSize(cu::Vector3f::Zero, cu::Vector3f(1.0f, 1.0f, 1.0f));

	cu::Vector2f test1(cu::rn::Random(-5000.0f, 5000.0f), 2423.5453253f);
	cu::Vector2f test2(cu::rn::Random(-5000.0f, 5000.0f), 21331.423025f);

	test1.Normalize(1.0f);
	test2.Normalize();

	std::cout << test1.x << test2.x;

	MenuStack stateStack(5);
	stateStack.RegisterState<TestState>("hello");
	stateStack.Push("hello");
	stateStack.ApplyPendingChanges();
	MenuState* menuState = stateStack.GetStateByID("hello");

	cu::StateMachine<std::string> stateMachine;
	stateMachine.AddState<TestState2>("hey");
	stateMachine.TransitionTo("hey");

	cu::Relation2DPtr relation1 = cu::Relation2D::Instantiate();
	cu::Relation2DPtr relation2 = cu::Relation2D::Instantiate();
	cu::Relation2DPtr relation3 = cu::Relation2D::Instantiate();

	cu::Relation2D::Attach(relation2, relation1);
	relation2->Move(cu::Vector2f(5.0f, 5.0f));

	const cu::Mat3f& globalMatrix = relation1->GetGlobalMatrix();

	constexpr cu::Vector3f vec1(1.0f, 2.0f, -6.0f);
	constexpr cu::Vector4f vec2 = vec1.XYZW(1.0f);

	constexpr cu::Sphere<float> sphere(cu::Vector3f(5.0f, 2.0f, -7.0f), 2.0f);
	constexpr cu::Shape::Type type = sphere.GetType();

	constexpr cu::Sphere<float> sphere2(cu::Vector3f(5.0f, 2.0f, -7.0f), 2.0f);

	for (int i = 0; i < 1000000; ++i)
	{
		auto result = cu::Collide<float>(sphere, sphere2);
	}

	cu::Vector2f test = cu::Vector2f::Up;

	cu::bm::End();

	std::string val = cu::st::TrimLeftCopy("     53");

	return 0;
}