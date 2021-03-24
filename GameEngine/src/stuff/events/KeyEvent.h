#pragma once

#include "Event.h"

#include <sstream>

namespace SOMEENGINE
{
	class SOME_API KeyEvent : public Event
	{
	public:
		inline int GetKeyCode() const { return _KeyCode; }

	public:
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)

	protected:
		KeyEvent(int keyCode) : _KeyCode(keyCode) {}
		int _KeyCode;
	};

	class SOME_API KeyPressedEvent : public KeyEvent
	{
	private:
		int _RepeatCount;

	public:
		KeyPressedEvent(int keyCode, int repeatCount)
			:
			KeyEvent(keyCode), _RepeatCount(repeatCount) {}

	public:
		inline int GetRepeatCount() const { return _RepeatCount; }
		
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyPressedEvent: " << _KeyCode << " (" << _RepeatCount << " repeats)";
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyPressed)
	};

	class SOME_API KeyReleasedEvent : public KeyEvent
	{
	public:
		KeyReleasedEvent(int keyCode)
			:
			KeyEvent(keyCode) {}

	public:
		std::string ToString()const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << _KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased)
	};
}