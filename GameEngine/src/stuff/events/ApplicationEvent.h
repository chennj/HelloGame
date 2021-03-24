#pragma once

#include "Event.h"

namespace SOMEENGINE
{
	class SOME_API WindowResizeEvent : public Event
	{
	private:
		unsigned int _width;
		unsigned int _height;

	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			:
			_width(width), _height(height)
		{
		}

	public:
		inline unsigned int GetWidth() const { return _width; }
		inline unsigned int GetHeight()const { return _height; }

	public:
		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "WindowResizeEvent: " << _width << ", " << _height;
			return ss.str();
		}

	public:
		EVENT_CLASS_TYPE(WindowResize)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class SOME_API WindowCloseEvent : public Event
	{
	public:
		WindowCloseEvent() {}

	public:
		EVENT_CLASS_TYPE(WindowClose)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class SOME_API AppTickEvent : public Event
	{
	public:
		AppTickEvent() {}

	public:
		EVENT_CLASS_TYPE(AppTick)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class SOME_API AppUpateEvent : public Event
	{
	public:
		AppUpateEvent() {}

	public:
		EVENT_CLASS_TYPE(AppUpate)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};

	class SOME_API AppRenderEvent : public Event
	{
	public:
		AppRenderEvent() {}

	public:
		EVENT_CLASS_TYPE(AppRender)
		EVENT_CLASS_CATEGORY(EventCategoryApplication)
	};
}