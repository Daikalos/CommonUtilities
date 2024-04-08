#pragma once

#include <unordered_map>
#include <variant>
#include <stdexcept>

#include <CommonUtilities/Utility/Traits.h>

#include "KeyboardInput.h"
#include "MouseInput.h"

namespace CommonUtilities
{
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	class InputBind final
	{
	public:
		using ButtonType = Bind;

		InputBind(KeyboardInput* aKeyboard = nullptr, MouseInput* aMouse = nullptr);
		~InputBind();

		NODISC const KeyboardInput* Keyboard() const noexcept;
		NODISC KeyboardInput* Keyboard() noexcept;

		NODISC const MouseInput* Mouse() const noexcept;
		NODISC MouseInput* Mouse() noexcept;

		NODISC bool IsKeyboardConnected() const noexcept;
		NODISC bool IsMouseConnected() const noexcept;

		NODISC bool GetEnabled() const noexcept;

		void Connect(KeyboardInput& aKeyboard);
		void Connect(MouseInput& aMouse);

		void DisconnectKeyboard();
		void DisconnectMouse();

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
		NODISC bool IsSet(const ButtonType& aBind);

		NODISC bool IsHeld(const ButtonType& aBind) const;
		NODISC bool IsPressed(const ButtonType& aBind) const;
		NODISC bool IsReleased(const ButtonType& aBind) const;

	private:
		using ButtonReg = std::variant<Keyboard::Key, Mouse::Button>;

		NODISC auto At(const ButtonType& aBind) const;
		NODISC auto At(const ButtonType& aBind);

		KeyboardInput* myKeyboard	{nullptr};
		MouseInput* myMouse			{nullptr};

		std::unordered_multimap<ButtonType, ButtonReg> myBinds;
		bool myEnabled {true};
	};

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline InputBind<Bind>::InputBind(KeyboardInput* aKeyboard, MouseInput* aMouse)
		: myKeyboard(aKeyboard), myMouse(aMouse)
	{ 

	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline InputBind<Bind>::~InputBind() = default;

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline const KeyboardInput* InputBind<Bind>::Keyboard() const noexcept
	{
		return myKeyboard;
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline KeyboardInput* InputBind<Bind>::Keyboard() noexcept
	{
		return myKeyboard;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline const MouseInput* InputBind<Bind>::Mouse() const noexcept
	{
		return myMouse;
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline MouseInput* InputBind<Bind>::Mouse() noexcept
	{
		return myMouse;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline bool InputBind<Bind>::IsKeyboardConnected() const noexcept
	{
		return myKeyboard != nullptr;
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline bool InputBind<Bind>::IsMouseConnected() const noexcept
	{
		return myMouse != nullptr;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline bool InputBind<Bind>::GetEnabled() const noexcept
	{
		return myEnabled;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::Connect(KeyboardInput& aKeyboard)
	{
		myKeyboard = &aKeyboard;
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::Connect(MouseInput& aMouse)
	{
		myMouse = &aMouse;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::DisconnectKeyboard()
	{
		myKeyboard = nullptr;
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::DisconnectMouse()
	{
		myMouse = nullptr;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::SetEnabled(bool aFlag)
	{
		myEnabled = aFlag;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::Set(const ButtonType& aBind, Keyboard::Key aKey)
	{
		myBinds.insert(std::make_pair(aBind, ButtonReg{ aKey }));
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline void InputBind<Bind>::Set(const ButtonType& aBind, Mouse::Button aButton)
	{
		myBinds.insert(std::make_pair(aBind, ButtonReg{ aButton }));
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

		auto range = this->At(aBind);

		for (auto it = range.first; it != range.second; ++it)
		{
			bool active = std::visit(tr::Overload
			{
				[this](Keyboard::Key aKey) { return IsKeyboardConnected() && myKeyboard->IsHeld(aKey); },
				[this](Mouse::Button aButton) { return IsMouseConnected() && myMouse->IsHeld(aButton); }
			}, it->second);

			if (active)
			{
				return true;
			}
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline bool InputBind<Bind>::IsPressed(const ButtonType& aBind) const
	{
		if (!GetEnabled())
			return false;

		auto range = this->At(aBind);

		for (auto it = range.first; it != range.second; ++it)
		{
			bool active = std::visit(tr::Overload
			{
				[this](Keyboard::Key aKey) { return IsKeyboardConnected() && myKeyboard->IsPressed(aKey); },
				[this](Mouse::Button aButton) { return IsMouseConnected() && myMouse->IsPressed(aButton); }
			}, it->second);

			if (active)
			{
				return true;
			}
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline bool InputBind<Bind>::IsReleased(const ButtonType& aBind) const
	{
		if (!GetEnabled())
			return false;

		auto range = this->At(aBind);

		for (auto it = range.first; it != range.second; ++it)
		{
			bool active = std::visit(tr::Overload
			{
				[this](Keyboard::Key aKey) { return IsKeyboardConnected() && myKeyboard->IsReleased(aKey); },
				[this](Mouse::Button aButton) { return IsMouseConnected() && myMouse->IsReleased(aButton); }
			}, it->second);

			if (active)
			{
				return true;
			}
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline auto InputBind<Bind>::At(const ButtonType& aBind) const
	{
		auto range = myBinds.equal_range(aBind);

		if (range.first == myBinds.end() && range.second == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		return range;
	}
	template<typename Bind> requires (!std::same_as<Bind, Keyboard::Key> && !std::same_as<Bind, Mouse::Button>)
	inline auto InputBind<Bind>::At(const ButtonType& aBind)
	{
		auto range = myBinds.equal_range(aBind);

		if (range.first == myBinds.end() && range.second == myBinds.end())
		{
			throw std::runtime_error("Bind could not be found");
		}

		return range;
	}
}