#pragma once

#include "Binds.hpp"
#include "KeyboardInput.h"
#include "MouseInput.h"

namespace CommonUtilities
{
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	class InputBind final
	{
	public:
		using ButtonType = Bind;

		explicit InputBind(const KeyboardInput* aKeyboard = nullptr, const MouseInput* aMouse = nullptr);
		~InputBind();

		NODISC const KeyboardInput* GetKeyboard() const;
		NODISC KeyboardInput* GetKeyboard();

		NODISC const MouseInput* GetMouse() const;
		NODISC MouseInput* GetMouse();

		NODISC bool GetEnabled() const noexcept;

		void SetKeyboard(const KeyboardInput* aKeyboard);

		void SetMouse(const MouseInput* aMouse);

		void SetEnabled(bool aFlag);

		///	Set the bind to match to key
		/// 
		void Set(const ButtonType& aBind, Keyboard::Key aKey);

		///	Set the bind to match to button
		/// 
		void Set(const ButtonType& aBind, Mouse::Button aButton);

		///	Remove an existing bind
		/// 
		void Remove(const ButtonType& aBind);

		///	\return Whether bind is set to some button
		/// 
		bool IsSet(const ButtonType& aBind);

		NODISC bool IsHeld(const ButtonType& aBind) const;
		NODISC bool IsPressed(const ButtonType& aBind) const;
		NODISC bool IsReleased(const ButtonType& aBind) const;

	private:
		enum class InputType
		{
			Mouse,
			Keyboard
		};

		struct ButtonReg
		{
			union
			{
				Keyboard::Key key;
				Mouse::Button button;
			};
			InputType type;
		};

		NODISC auto At(const ButtonType& aBind) -> ButtonReg&;
		NODISC auto At(const ButtonType& aBind) const -> const ButtonReg&;

		const KeyboardInput* myKeyboard {nullptr};
		const MouseInput* myMouse		{nullptr};

		std::unordered_map<ButtonType, ButtonReg> myBinds;
		bool myEnabled {true};
	};

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline InputBind<Bind>::InputBind(const KeyboardInput* aKeyboard, const MouseInput* aMouse)
		: myKeyboard(aKeyboard), myMouse(aMouse)
	{ 

	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline InputBind<Bind>::~InputBind() = default;

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline const KeyboardInput* InputBind<Bind>::GetKeyboard() const
	{
		return myKeyboard;
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline KeyboardInput* InputBind<Bind>::GetKeyboard()
	{
		return myKeyboard;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline const MouseInput* InputBind<Bind>::GetMouse() const
	{
		return myMouse;
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline MouseInput* InputBind<Bind>::GetMouse()
	{
		return myMouse;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline bool InputBind<Bind>::GetEnabled() const noexcept
	{
		return myEnabled;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::SetKeyboard(const KeyboardInput* aKeyboard)
	{
		myKeyboard = aKeyboard;
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::SetMouse(const MouseInput* aMouse)
	{
		myMouse = aMouse;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::SetEnabled(bool aFlag)
	{
		myEnabled = aFlag;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::Set(const ButtonType& aBind, Keyboard::Key aKey)
	{
		ButtonReg reg{};
		reg.key = aKey;
		reg.type = InputType::Keyboard;

		myBinds[aBind] = reg;
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::Set(const ButtonType& aBind, Mouse::Button aButton)
	{
		ButtonReg reg{};
		reg.button = aButton;
		reg.type = InputType::Mouse;

		myBinds[aBind] = reg;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::Remove(const ButtonType& aBind)
	{
		const auto it = myBinds.find(aBind);
		if (it == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		myBinds.erase(it);
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline bool InputBind<Bind>::IsSet(const ButtonType& aBind)
	{
		const auto it = myBinds.find(aBind);
		return it != myBinds.end();
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline bool InputBind<Bind>::IsHeld(const ButtonType& aBind) const
	{
		if (!GetEnabled())
			return false;

		const ButtonReg& reg = At(aBind);

		switch (reg.type)
		{
			case InputType::Keyboard: return GetKeyboard() && GetKeyboard()->IsHeld(reg.key);
			case InputType::Mouse: return GetMouse() && GetMouse()->IsHeld(reg.button);
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline bool InputBind<Bind>::IsPressed(const ButtonType& aBind) const
	{
		if (!GetEnabled())
			return false;

		const ButtonReg& reg = At(aBind);

		switch (reg.type)
		{
			case InputType::Keyboard: return GetKeyboard() && GetKeyboard()->IsPressed(reg.key);
			case InputType::Mouse: return GetMouse() && GetMouse()->IsPressed(reg.button);
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline bool InputBind<Bind>::IsReleased(const ButtonType& aBind) const
	{
		if (!GetEnabled())
			return false;

		const ButtonReg& reg = At(aBind);

		switch (reg.type)
		{
			case InputType::Keyboard: return GetKeyboard() && GetKeyboard()->IsReleased(reg.key);
			case InputType::Mouse: return GetMouse() && GetMouse()->IsReleased(reg.button);
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline auto InputBind<Bind>::At(const ButtonType& aBind) -> ButtonReg&
	{
		return const_cast<ButtonReg&>(std::as_const(*this).At(aBind));
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline auto InputBind<Bind>::At(const ButtonType& aBind) const -> const ButtonReg&
	{
		const auto it = myBinds.find(aBind);
		if (it == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		return it->second;
	}

}