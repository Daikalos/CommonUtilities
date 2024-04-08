#pragma once

#include "Binds.hpp"
#include "MouseInput.h"

namespace CommonUtilities
{
	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	class MouseBind final : public Binds<Bind, Mouse::Button>
	{
	public:
		using ButtonType = Bind;

		MouseBind(const MouseInput* aMouse = nullptr);
		~MouseBind() = default;

		NODISC const MouseInput* Mouse() const noexcept;
		NODISC MouseInput* Mouse() noexcept;

		NODISC bool IsConnected() const noexcept;

		void Connect(const MouseInput& aMouse);
		void Disconnect();

		NODISC bool IsHeld(const ButtonType& aBind) const;
		NODISC bool IsPressed(const ButtonType& aBind) const;
		NODISC bool IsReleased(const ButtonType& aBind) const;

	private:
		const MouseInput* myMouse {nullptr};
	};

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline MouseBind<Bind>::MouseBind(const MouseInput* aMouse)
		: myMouse(aMouse) { }

	template<typename Bind>
	inline const MouseInput* MouseBind<Bind>::Mouse() const noexcept
	{
		return myMouse;
	}
	template<typename Bind>
	inline MouseInput* MouseBind<Bind>::Mouse() noexcept
	{
		return myMouse;
	}

	template<typename Bind>
	inline bool MouseBind<Bind>::IsConnected() const noexcept
	{
		return myMouse != nullptr;
	}

	template<typename Bind>
	inline void MouseBind<Bind>::Connect(const MouseInput& aMouse)
	{
		myMouse = &aMouse;
	}
	template<typename Bind>
	inline void MouseBind<Bind>::Disconnect()
	{
		myMouse = nullptr;
	}

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline bool MouseBind<Bind>::IsHeld(const ButtonType& aBind) const
	{
		if (!this->GetEnabled() || !IsConnected())
			return false;

		auto range = this->At(aBind);

		for (auto it = range.first; it != range.second; ++it)
		{
			if (myMouse->IsHeld(it->second))
			{
				return true;
			}
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline bool MouseBind<Bind>::IsPressed(const ButtonType& aBind) const
	{
		if (!this->GetEnabled() || !IsConnected())
			return false;

		auto range = this->At(aBind);

		for (auto it = range.first; it != range.second; ++it)
		{
			if (myMouse->IsPressed(it->second))
			{
				return true;
			}
		}

		return false;
	}

	template<typename Bind> requires (!std::same_as<Bind, Mouse::Button>)
	inline bool MouseBind<Bind>::IsReleased(const ButtonType& aBind) const
	{
		if (!this->GetEnabled() || !IsConnected())
			return false;

		auto range = this->At(aBind);

		for (auto it = range.first; it != range.second; ++it)
		{
			if (myMouse->IsReleased(it->second))
			{
				return true;
			}
		}

		return false;
	}
}