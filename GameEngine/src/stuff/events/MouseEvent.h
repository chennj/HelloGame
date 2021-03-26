#pragma once

#include "Event.h"

namespace SOMEENGINE
{
	class SOME_API MouseMovedEvent : public Event
	{
	private:
		float _MouseX;
		float _MouseY;

	public:
		MouseMovedEvent(float x, float y)
			:
			_MouseX(x), _MouseY(y) {}

	public:
		inline float GetX()const { return _MouseX; }
		inline float GetY()const { return _MouseY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseMovedEvent: " << _MouseX << ", " << _MouseY;
			return ss.str();
		}

	public:
		EVENT_CLASS_TYPE(MouseMoved)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class SOME_API MouseScrolledEvent : public Event
	{
	private:
		float _XOffset;
		float _YOffset;

	public:
		MouseScrolledEvent(float xOffset, float yOffset)
			:
			_XOffset(xOffset), _YOffset(yOffset) {}

	public:
		inline float GetXOffset()const { return _XOffset; }
		inline float GetYOffset()const { return _YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << _XOffset << ", " << _YOffset;
			return ss.str();
		}

	public:
		EVENT_CLASS_TYPE(MouseScrolled)
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class SOME_API MouseButtonEvent : public Event
	{
	private:
		int _Button;

	public:
		MouseButtonEvent(int button)
			:
			_Button(button) {}

	public:
		inline int GetMouseButton()const { return _Button; }

	public:
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
	};

	class SOME_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			:
			MouseButtonEvent(button) {}

	public:
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << GetMouseButton();
			return ss.str();
		}

	public:
		EVENT_CLASS_TYPE(MouseButtonPressed)
	};

	class SOME_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			:
			MouseButtonEvent(button) {}

	public:
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << GetMouseButton();
			return ss.str();
		}

	public:
		EVENT_CLASS_TYPE(MouseButtonReleased)
	};
}